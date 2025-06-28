#include <benchmark/benchmark.h>
#include <sw/redis++/redis++.h>
#include <string>

static void BM_RedisPlusPlus_Decr(benchmark::State& state) {
    std::string host = std::getenv("REDIS_HOST") ? std::getenv("REDIS_HOST") : "127.0.0.1";
    std::string port = std::getenv("REDIS_PORT") ? std::getenv("REDIS_PORT") : "6379";
    std::string connection = "tcp://" + host + ":" + port;

    try {
        sw::redis::Redis redis(connection);
        std::string key = "test_counter";
        // Set the key to a value before the benchmark loop to ensure it exists
        redis.set(key, "100");

        for (auto _ : state) {
            // Decrement the counter
            auto result = redis.decr(key);
            // Ensure the response is processed to avoid optimization out
            benchmark::DoNotOptimize(result);
        }
        // Reset the key after the benchmark to avoid negative values in subsequent runs
        redis.set(key, "100");
    } catch (const std::exception& e) {
        state.SkipWithError(e.what());
    }
}

BENCHMARK(BM_RedisPlusPlus_Decr);
