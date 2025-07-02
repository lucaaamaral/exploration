#include <benchmark/benchmark.h>
#include <redis-cpp/stream.h>
#include <redis-cpp/execute.h>
#include <string>

static void BM_RedisCpp_Rpop(benchmark::State& state) {
    std::string host = std::getenv("REDIS_HOST") ? std::getenv("REDIS_HOST") : "127.0.0.1";
    std::string port = std::getenv("REDIS_PORT") ? std::getenv("REDIS_PORT") : "6379";

    try {
        std::shared_ptr<std::iostream> stream = rediscpp::make_stream(host, port);
        std::string key = "test_list";
        std::string value = "test_value";
        // Push some values to the list before the benchmark loop to ensure it has elements
        rediscpp::execute(*stream, "RPUSH", key, value);
        rediscpp::execute(*stream, "RPUSH", key, value);
        rediscpp::execute(*stream, "RPUSH", key, value);

        for (auto _ : state) {
            // Pop a value from the right of the list
            auto response = rediscpp::execute(*stream, "RPOP", key);
            // Ensure the response is processed to avoid optimization out
            benchmark::DoNotOptimize(response);
            // Repush a value to maintain list size for consistent benchmarking
            rediscpp::execute(*stream, "RPUSH", key, value);
        }
        // Clean up the key after the benchmark
        rediscpp::execute(*stream, "DEL", key);
    } catch (const std::exception& e) {
        state.SkipWithError(e.what());
    }
}

BENCHMARK(BM_RedisCpp_Rpop);
