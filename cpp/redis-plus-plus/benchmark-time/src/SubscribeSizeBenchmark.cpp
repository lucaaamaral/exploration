#include <BenchmarkRunner.h>
#include <string>
#include <thread>
#include <atomic>
#include <sw/redis++/redis++.h>

void subscribeIncreasingSize(const benchmark::BenchmarkConfig& config, benchmark::Statistics& stats) {
    std::string host = std::getenv("REDIS_HOST") ? std::getenv("REDIS_HOST") : "127.0.0.1";
    std::string port = std::getenv("REDIS_PORT") ? std::getenv("REDIS_PORT") : "6379";
    std::string connection = "tcp://" + host + ":" + port;
    auto redis = sw::redis::Redis(connection);
    size_t received_count = 0;
    std::atomic<bool> stop_flag(false);
    std::atomic<bool> msg_received(false);
    std::string channel("test_chan_size");
    
    auto subscriber = redis.subscriber();
    subscriber.on_message([&stats, &received_count, &stop_flag, &msg_received, &config](std::string channel, std::string message) {
        if (received_count < config.iterations) {
            received_count++;
            msg_received = true;
            if (received_count == config.iterations) {
                stop_flag = true;
            }
        }
    });
    
    subscriber.subscribe(channel);
    std::thread subscriber_thread([&subscriber, &stop_flag]() {
        while (!stop_flag) {
            try {
                subscriber.consume();
            } catch (const std::exception& e) {
                break;
            }
        }
    });
    
    size_t current_size = 16;
    size_t max_size = 1048576; // 1MB
    size_t size_multiplier = 2;
    size_t iterations_per_size = config.iterations / log2(max_size / 16);
    if (iterations_per_size == 0) iterations_per_size = 1;
    std::string message;
    size_t iteration_count = 0;
    
    for (size_t i = 0; i < config.iterations; ++i) {
        if (iteration_count % iterations_per_size == 0 && current_size < max_size) {
            current_size *= size_multiplier;
            if (current_size > max_size) current_size = max_size;
            message = std::string(current_size, '-');
        }
        stats.start_timer();
        redis.publish(channel, message);
        while (!msg_received) std::this_thread::sleep_for(std::chrono::nanoseconds(10));
        stats.stop_timer();
        msg_received = false;
        iteration_count++;
    }
    
    auto start_time = std::chrono::steady_clock::now();
    while (received_count < config.iterations &&
           std::chrono::steady_clock::now() - start_time < std::chrono::seconds(30)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    stop_flag = true;
    if (subscriber_thread.joinable()) {
        subscriber_thread.join();
    }
}

REGISTER_BENCHMARK(SubscribeIncreasingSize, subscribeIncreasingSize);
