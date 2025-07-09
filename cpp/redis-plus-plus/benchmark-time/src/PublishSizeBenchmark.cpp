#include <BenchmarkRunner.h>
#include <string>
#include <sw/redis++/redis++.h>

void publishIncreasingSize(const benchmark::BenchmarkConfig& config, benchmark::Statistics& stats) {
    std::string host = std::getenv("REDIS_HOST") ? std::getenv("REDIS_HOST") : "127.0.0.1";
    std::string port = std::getenv("REDIS_PORT") ? std::getenv("REDIS_PORT") : "6379";
    std::string connection = "tcp://" + host + ":" + port;
    auto redis = sw::redis::Redis(connection);
    std::string channel("test_chan_size");
    size_t current_size = 16;
    size_t max_size = 1048576; // 1MB
    size_t iterations_per_size = config.iterations / log2(max_size / 16);
    if (iterations_per_size == 0) iterations_per_size = 1;
    std::string message;
    for (size_t i = 0; i < config.iterations; ++i) {
        if (i % iterations_per_size == 0 && current_size < max_size) {
            if (current_size > max_size) current_size = max_size;
            message = std::string(current_size, '-');
            current_size *= 2;
        }
        stats.start_timer();
        redis.publish(channel, message);
        stats.stop_timer();
    }
}

REGISTER_BENCHMARK(PublishIncreasingSize, publishIncreasingSize);
