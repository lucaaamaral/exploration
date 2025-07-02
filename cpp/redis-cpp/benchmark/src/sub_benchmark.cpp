#include <benchmark/benchmark.h>
#include <redis-cpp/stream.h>
#include <redis-cpp/execute.h>
#include <redis-cpp/value.h>
#include <string>
#include <atomic>
#include <thread>
#include <chrono>

static void BM_RedisCpp_Subscribe(benchmark::State& state) {
    std::string host = std::getenv("REDIS_HOST") ? std::getenv("REDIS_HOST") : "127.0.0.1";
    std::string port = std::getenv("REDIS_PORT") ? std::getenv("REDIS_PORT") : "6379";

    try {
        // Create two streams: one for publishing, one for subscribing
        std::shared_ptr<std::iostream> stream_pub = rediscpp::make_stream(host, port);
        std::shared_ptr<std::iostream> stream_sub = rediscpp::make_stream(host, port);
        std::string queue_name = "benchmark:pubsub";
        std::string message = "test_message_for_benchmark";
        std::atomic<long> received_count(0);
        bool volatile stopped = false;

        // Start subscriber in a separate thread
        std::thread sub_thread([&stream_sub, &queue_name, &received_count, &stopped]() {
            try {
                auto response = rediscpp::execute(*stream_sub, "SUBSCRIBE", queue_name);
                while (!stopped) {
                    rediscpp::value value(*stream_sub);
                    received_count++;
                }
            } catch (const std::exception& e) {
                // std::cerr << "Subscriber error: " << e.what() << std::endl;
            }
        });

        // Give subscriber time to start
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        for (auto _ : state) {
            // Publish a message
            rediscpp::execute(*stream_pub, "PUBLISH", queue_name, message);
            // Wait briefly for the message to be received
            std::this_thread::sleep_for(std::chrono::microseconds(10));
            // Record the received count to prevent optimization
            benchmark::DoNotOptimize(received_count.load());
        }

        // Cleanup: stop subscriber thread
        stopped = true;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if (sub_thread.joinable()) {
            sub_thread.detach();
        }
    } catch (const std::exception& e) {
        state.SkipWithError(e.what());
    }
}

BENCHMARK(BM_RedisCpp_Subscribe);
