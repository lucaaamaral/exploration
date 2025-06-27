#include <benchmark/benchmark.h>
#include <sw/redis++/redis++.h>
#include <string>
#include <vector>

static void BM_RedisPlusPlus_Hmget(benchmark::State& state) {
    std::string host = std::getenv("REDIS_HOST") ? std::getenv("REDIS_HOST") : "127.0.0.1";
    std::string port = std::getenv("REDIS_PORT") ? std::getenv("REDIS_PORT") : "6379";
    std::string connection = "tcp://" + host + ":" + port;

    try {
        sw::redis::Redis redis(connection);
        std::string key = "test_hash";
        std::vector<std::pair<std::string, std::string>> fields = {
            {"field1", "value1"},
            {"field2", "value2"},
            {"field3", "value3"}
        };
        // Set multiple fields before the benchmark loop to ensure they exist
        redis.hmset(key, fields.begin(), fields.end());
        std::vector<std::string> field_names = {"field1", "field2", "field3"};
        std::vector<sw::redis::OptionalString> results(field_names.size());

        for (auto _ : state) {
            // Get multiple field values from a hash
            redis.hmget(key, field_names.begin(), field_names.end(), results.begin());
            // Ensure the response is processed to avoid optimization out
            benchmark::DoNotOptimize(results);
        }
        // Clean up the key after the benchmark
        redis.del(key);
    } catch (const std::exception& e) {
        state.SkipWithError(e.what());
    }
}

BENCHMARK(BM_RedisPlusPlus_Hmget);
