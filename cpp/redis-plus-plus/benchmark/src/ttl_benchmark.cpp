#include <benchmark/benchmark.h>
#include <sw/redis++/redis++.h>
#include <string>

static void BM_RedisPlusPlus_Ttl(benchmark::State& state) {
    std::string host = std::getenv("REDIS_HOST") ? std::getenv("REDIS_HOST") : "127.0.0.1";
    std::string port = std::getenv("REDIS_PORT") ? std::getenv("REDIS_PORT") : "6379";
    std::string connection = "tcp://" + host + ":" + port;

    try {
        sw::redis::Redis redis(connection);
        std::string key = "test_key";
        std::string value = "test_value";
        // Set the key and expiration time before the benchmark loop to ensure it exists
        redis.set(key, value);
        redis.expire(key, 3600);

        for (auto _ : state) {
            // Get the remaining time to live for the key
            auto result = redis.ttl(key);
            // Ensure the response is processed to avoid optimization out
            benchmark::DoNotOptimize(result);
        }
    } catch (const std::exception& e) {
        state.SkipWithError(e.what());
    }
}

BENCHMARK(BM_RedisPlusPlus_Ttl);
