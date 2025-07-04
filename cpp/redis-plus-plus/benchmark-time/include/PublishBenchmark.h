#ifndef BENCHMARK_TIME_PUBLISH_BENCHMARK_H
#define BENCHMARK_TIME_PUBLISH_BENCHMARK_H

#include <string>
#include <sw/redis++/redis++.h>
#include "Statistics.h"
#include "BenchmarkConfig.h"

class PublishBenchmark {
public:
    PublishBenchmark(BenchmarkConfig config)
        : iterations_(config.iterations) {}

    // Run the publish benchmark and collect timing data
    void run(Statistics& stats) {
        // Get Redis connection parameters from environment variables
        std::string host = std::getenv("REDIS_HOST") ? std::getenv("REDIS_HOST") : "127.0.0.1";
        std::string port = std::getenv("REDIS_PORT") ? std::getenv("REDIS_PORT") : "6379";
        std::string connection = "tcp://" + host + ":" + port;
        // Set up individual Redis connection for this test
        auto redis = sw::redis::Redis(connection);
        // Set up channel and message variables for this test
        std::string channel("test_chan");
        std::string message("test_msg");
        for (size_t i = 0; i < this->iterations_; ++i) {
            stats.start_timer();
            redis.publish(channel, message);
            stats.stop_timer();
        }
    }

private:
    size_t iterations_;
};

#endif // BENCHMARK_TIME_PUBLISH_BENCHMARK_H
