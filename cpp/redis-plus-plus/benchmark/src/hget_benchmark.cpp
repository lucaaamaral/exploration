#include <benchmark/benchmark.h>
#include <sw/redis++/redis++.h>
#include <string>

static void BM_RedisPlusPlus_Hget(benchmark::State& state) {
    std::string host = std::getenv("REDIS_HOST") ? std::getenv("REDIS_HOST") : "127.0.0.1";
    std::string port = std::getenv("REDIS_PORT") ? std::getenv("REDIS_PORT") : "6379";
    std::string connection = "tcp://" + host + ":" + port;

    try {
        sw::redis::Redis redis(connection);
        std::string key = "test_hash";
        std::string field = "test_field";
        std::string value = "test_value";
        // Set the hash field before the benchmark loop to ensure it exists
        redis.hset(key, field, value);

        for (auto _ : state) {
            // Get a field value from a hash
            auto result = redis.hget(key, field);
            // Ensure the response is processed to avoid optimization out
            benchmark::DoNotOptimize(result);
        }
        // Clean up the key after the benchmark
        redis.del(key);
    } catch (const std::exception& e) {
        state.SkipWithError(e.what());
    }
}

BENCHMARK(BM_RedisPlusPlus_Hget);
