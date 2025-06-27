#include <benchmark/benchmark.h>
#include <sw/redis++/redis++.h>
#include <string>

static void BM_RedisPlusPlus_Lpop(benchmark::State& state) {
    std::string host = std::getenv("REDIS_HOST") ? std::getenv("REDIS_HOST") : "127.0.0.1";
    std::string port = std::getenv("REDIS_PORT") ? std::getenv("REDIS_PORT") : "6379";
    std::string connection = "tcp://" + host + ":" + port;

    try {
        sw::redis::Redis redis(connection);
        std::string key = "test_list";
        std::string value = "test_value";
        // Push some values to the list before the benchmark loop to ensure it has elements
        redis.lpush(key, value);
        redis.lpush(key, value);
        redis.lpush(key, value);

        for (auto _ : state) {
            // Pop a value from the left of the list
            auto result = redis.lpop(key);
            // Ensure the response is processed to avoid optimization out
            benchmark::DoNotOptimize(result);
            // Repush a value to maintain list size for consistent benchmarking
            redis.lpush(key, value);
        }
        // Clean up the key after the benchmark
        redis.del(key);
    } catch (const std::exception& e) {
        state.SkipWithError(e.what());
    }
}

BENCHMARK(BM_RedisPlusPlus_Lpop);
