#include <benchmark/benchmark.h>
#include <sw/redis++/redis++.h>
#include <string>

static void BM_RedisPlusPlus_Publish(benchmark::State& state) {
    std::string host = std::getenv("REDIS_HOST") ? std::getenv("REDIS_HOST") : "127.0.0.1";
    std::string port = std::getenv("REDIS_PORT") ? std::getenv("REDIS_PORT") : "6379";
    std::string connection = "tcp://" + host + ":" + port;

    try {
        sw::redis::Redis redis(connection);
        std::string channel = "benchmark:pubsub";
        std::string message = "test_message_for_benchmark";

        for (auto _ : state) {
            auto result = redis.publish(channel, message);
            // Ensure the response is processed to avoid optimization out
            benchmark::DoNotOptimize(result);
        }
    } catch (const std::exception& e) {
        state.SkipWithError(e.what());
    }
}

BENCHMARK(BM_RedisPlusPlus_Publish);
