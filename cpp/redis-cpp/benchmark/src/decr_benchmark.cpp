#include <benchmark/benchmark.h>
#include <redis-cpp/stream.h>
#include <redis-cpp/execute.h>
#include <string>

static void BM_RedisCpp_Decr(benchmark::State& state) {
    std::string host = std::getenv("REDIS_HOST") ? std::getenv("REDIS_HOST") : "127.0.0.1";
    std::string port = std::getenv("REDIS_PORT") ? std::getenv("REDIS_PORT") : "6379";

    try {
        std::shared_ptr<std::iostream> stream = rediscpp::make_stream(host, port);
        std::string key = "test_counter";
        // Set the key to a value before the benchmark loop to ensure it exists
        rediscpp::execute(*stream, "SET", key, "100");

        for (auto _ : state) {
            // Decrement the counter
            auto response = rediscpp::execute(*stream, "DECR", key);
            // Ensure the response is processed to avoid optimization out
            benchmark::DoNotOptimize(response);
        }
        // Reset the key after the benchmark to avoid negative values in subsequent runs
        rediscpp::execute(*stream, "SET", key, "100");
    } catch (const std::exception& e) {
        state.SkipWithError(e.what());
    }
}

BENCHMARK(BM_RedisCpp_Decr);
