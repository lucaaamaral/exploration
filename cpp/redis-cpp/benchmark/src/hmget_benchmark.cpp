#include <benchmark/benchmark.h>
#include <redis-cpp/stream.h>
#include <redis-cpp/execute.h>
#include <string>

static void BM_RedisCpp_Hmget(benchmark::State& state) {
    std::string host = std::getenv("REDIS_HOST") ? std::getenv("REDIS_HOST") : "127.0.0.1";
    std::string port = std::getenv("REDIS_PORT") ? std::getenv("REDIS_PORT") : "6379";

    try {
        std::shared_ptr<std::iostream> stream = rediscpp::make_stream(host, port);
        std::string key = "test_hash";
        // Set multiple fields before the benchmark loop to ensure they exist
        rediscpp::execute(*stream, "HMSET", key, "field1", "value1", "field2", "value2", "field3", "value3");

        for (auto _ : state) {
            // Get multiple field values from a hash
            auto response = rediscpp::execute(*stream, "HMGET", key, "field1", "field2", "field3");
            // Ensure the response is processed to avoid optimization out
            benchmark::DoNotOptimize(response);
        }
        // Clean up the key after the benchmark
        rediscpp::execute(*stream, "DEL", key);
    } catch (const std::exception& e) {
        state.SkipWithError(e.what());
    }
}

BENCHMARK(BM_RedisCpp_Hmget);
