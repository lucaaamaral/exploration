#include <benchmark/benchmark.h>
#include <sw/redis++/redis++.h>
#include <string>
#include <atomic>
#include <thread>
#include <chrono>

static void BM_RedisPlusPlus_Subscribe(benchmark::State& state) {
    std::string host = std::getenv("REDIS_HOST") ? std::getenv("REDIS_HOST") : "127.0.0.1";
    std::string port = std::getenv("REDIS_PORT") ? std::getenv("REDIS_PORT") : "6379";
    std::string connection = "tcp://" + host + ":" + port;

    try {
        sw::redis::Redis redis_pub(connection);
        sw::redis::Redis redis_sub(connection);
        std::string channel = "benchmark:pubsub";
        std::string message = "test_message_for_benchmark";
        std::atomic<long> received_count(0);
        std::atomic<bool> message_received(false);

        // Subscriber callback to count received messages and flag reception
        auto callback = [&received_count, &message_received](const std::string& chan, const std::string& msg) {
            received_count++;
            message_received = true;
        };

        // Start subscriber in a separate thread
        std::thread sub_thread([&redis_sub, &channel, &callback]() {
            try {
                auto sub = redis_sub.subscriber();
                sub.on_message(callback);
                sub.subscribe(channel);
                while (true) {
                    sub.consume();
                }
            } catch (const std::exception& e) {
                // Log error but continue
                // std::cerr << "Subscriber error: " << e.what() << std::endl;
            }
        });

        // Give subscriber time to start
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        for (auto _ : state) {
            // Reset reception flag
            message_received = false;
            // Publish a message
            redis_pub.publish(channel, message);
            // Wait briefly for the message to be received
            auto start_time = std::chrono::steady_clock::now();
            while (!message_received && std::chrono::steady_clock::now() - start_time < std::chrono::milliseconds(100)) {
                std::this_thread::sleep_for(std::chrono::microseconds(1));
            }
            // Record the received count to prevent optimization
            benchmark::DoNotOptimize(received_count.load());
        }

        // Cleanup: detach subscriber thread (it will be terminated when program ends)
        if (sub_thread.joinable()) {
            sub_thread.detach();
        }
    } catch (const std::exception& e) {
        state.SkipWithError(e.what());
    }
}

BENCHMARK(BM_RedisPlusPlus_Subscribe);
