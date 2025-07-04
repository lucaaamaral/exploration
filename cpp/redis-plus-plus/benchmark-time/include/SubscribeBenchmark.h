#ifndef BENCHMARK_TIME_SUBSCRIBE_BENCHMARK_H
#define BENCHMARK_TIME_SUBSCRIBE_BENCHMARK_H

#include <string>
#include <thread>
#include <atomic>
#include <sw/redis++/redis++.h>
#include "Statistics.h"
#include "BenchmarkConfig.h"

class SubscribeBenchmark {
public:
    SubscribeBenchmark(BenchmarkConfig config)
        : iterations_(config.iterations), channel_("test_chan") {}

    // Run the subscribe benchmark and collect timing data
    void run(Statistics& stats) {
        // Get Redis connection parameters from environment variables
        std::string host = std::getenv("REDIS_HOST") ? std::getenv("REDIS_HOST") : "127.0.0.1";
        std::string port = std::getenv("REDIS_PORT") ? std::getenv("REDIS_PORT") : "6379";
        std::string connection = "tcp://" + host + ":" + port;
        // Set up individual Redis connection for this test
        auto redis = sw::redis::Redis(connection);
        size_t received_count = 0;
        std::atomic<bool> stop_flag(false);
        std::atomic<bool> msg_received(false);

        // Create a subscriber
        auto subscriber = redis.subscriber();
        subscriber.on_message([&stats, &received_count, &stop_flag, &msg_received, this](std::string channel, std::string message) {
            if (received_count < this->iterations_) {
                received_count++;
                msg_received = true;
                if (received_count == this->iterations_) {
                    stop_flag = true;
                }
            }
        });

        // Subscribe to the channel
        subscriber.subscribe(this->channel_);

        // Start a separate thread to handle subscription messages
        std::thread subscriber_thread([&subscriber, &stop_flag]() {
            while (!stop_flag) {
                try {
                    subscriber.consume();
                } catch (const std::exception& e) {
                    // Handle exceptions if needed
                    break;
                }
            }
        });

        // Start timer for each publish to measure time from publish to receipt
        for (size_t i = 0; i < this->iterations_; ++i) {
            stats.start_timer(); // Start timing before publish
            redis.publish(this->channel_, "test_message");
            while (!msg_received) std::this_thread::sleep_for(std::chrono::nanoseconds(10));
            stats.stop_timer();
            msg_received = false; // Reset for the next iteration
        }

        // Wait for all messages to be received or timeout
        auto start_time = std::chrono::steady_clock::now();
        while (received_count < this->iterations_ &&
               std::chrono::steady_clock::now() - start_time < std::chrono::seconds(30)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        stop_flag = true;
        if (subscriber_thread.joinable()) {
            subscriber_thread.join();
        }
    }

private:
    size_t iterations_;
    std::string channel_;
};

#endif // BENCHMARK_TIME_SUBSCRIBE_BENCHMARK_H
