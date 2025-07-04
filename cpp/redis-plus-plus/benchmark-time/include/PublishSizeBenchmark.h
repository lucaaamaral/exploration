#ifndef BENCHMARK_TIME_PUBLISH_SIZE_BENCHMARK_H
#define BENCHMARK_TIME_PUBLISH_SIZE_BENCHMARK_H

#include <string>
#include <vector>
#include <sw/redis++/redis++.h>
#include "Statistics.h"
#include "BenchmarkConfig.h"

class PublishSizeBenchmark {
public:
    PublishSizeBenchmark(BenchmarkConfig config)
        : iterations_(config.iterations), 
          initial_size_(16), 
          max_size_(1048576), // 1MB as maximum size
          size_multiplier_(2) {}

    // Run the publish benchmark with increasing message sizes
    void run(Statistics& stats) {
        // Get Redis connection parameters from environment variables
        std::string host = std::getenv("REDIS_HOST") ? std::getenv("REDIS_HOST") : "127.0.0.1";
        std::string port = std::getenv("REDIS_PORT") ? std::getenv("REDIS_PORT") : "6379";
        std::string connection = "tcp://" + host + ":" + port;
        // Set up individual Redis connection for this test
        auto redis = sw::redis::Redis(connection);
        // Set up channel for this test
        std::string channel("test_chan_size");
        // Start with initial message size
        size_t current_size = this->initial_size_;
        std::string message;
        size_t iterations_per_size = this->iterations_ / log2(this->max_size_ / this->initial_size_);
        if (iterations_per_size == 0) iterations_per_size = 1;
        for (size_t i = 0; i < this->iterations_; ++i) {
            if (i % iterations_per_size == 0 && current_size < this->max_size_) {
                if (current_size > this->max_size_) current_size = this->max_size_;
                message = std::string(current_size, '-');
            }
            stats.start_timer();
            redis.publish(channel, message);
            stats.stop_timer();
        }
    }

private:
    size_t iterations_;
    size_t initial_size_;
    size_t max_size_;
    size_t size_multiplier_;
};

#endif // BENCHMARK_TIME_PUBLISH_SIZE_BENCHMARK_H
