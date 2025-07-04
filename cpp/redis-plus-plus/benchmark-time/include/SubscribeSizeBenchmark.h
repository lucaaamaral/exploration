#ifndef BENCHMARK_TIME_SUBSCRIBE_SIZE_BENCHMARK_H
#define BENCHMARK_TIME_SUBSCRIBE_SIZE_BENCHMARK_H

#include <string>
#include <thread>
#include <atomic>
#include <sw/redis++/redis++.h>
#include "Statistics.h"
#include "BenchmarkConfig.h"

class SubscribeSizeBenchmark {
public:
    SubscribeSizeBenchmark(BenchmarkConfig config)
        : iterations_(config.iterations), 
          channel_("test_chan_size"), 
          initial_size_(16), 
          max_size_(1048576), // 1MB as maximum size
          size_multiplier_(2) {}

    // Run the subscribe benchmark with increasing message sizes
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

        // Start with initial message size
        size_t current_size = this->initial_size_;
        std::string message;
        size_t iterations_per_size = this->iterations_ / log2(this->max_size_ / this->initial_size_);
        if (iterations_per_size == 0) iterations_per_size = 1;

        // Start timer for each publish to measure time from publish to receipt
        size_t iteration_count = 0;
        for (size_t i = 0; i < this->iterations_; ++i) {
            if (iteration_count % iterations_per_size == 0 && current_size < this->max_size_) {
                current_size *= this->size_multiplier_;
                if (current_size > this->max_size_) current_size = this->max_size_;
                message = std::string(current_size, '-');
            }
            stats.start_timer(); // Start timing before publish
            redis.publish(this->channel_, message);
            while (!msg_received) std::this_thread::sleep_for(std::chrono::nanoseconds(10));
            stats.stop_timer();
            msg_received = false; // Reset for the next iteration
            iteration_count++;
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
    size_t initial_size_;
    size_t max_size_;
    size_t size_multiplier_;

};

#endif // BENCHMARK_TIME_SUBSCRIBE_SIZE_BENCHMARK_H
