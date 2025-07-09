#include <BenchmarkRunner.h>
#include <string>
#include <sw/redis++/redis++.h>

void publishFixedSize(const benchmark::BenchmarkConfig& config, benchmark::Statistics& stats) {
    std::string host = std::getenv("REDIS_HOST") ? std::getenv("REDIS_HOST") : "127.0.0.1";
    std::string port = std::getenv("REDIS_PORT") ? std::getenv("REDIS_PORT") : "6379";
    std::string connection = "tcp://" + host + ":" + port;
    auto redis = sw::redis::Redis(connection);
    std::string channel("test_chan");
    std::string message("test_msg");
    for (size_t i = 0; i < config.iterations; ++i) {
        stats.start_timer();
        redis.publish(channel, message);
        stats.stop_timer();
    }
}

REGISTER_BENCHMARK(PublishFixedSize, publishFixedSize);
