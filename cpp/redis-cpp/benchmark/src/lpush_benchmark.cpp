#include <benchmark/benchmark.h>
#include <redis-cpp/stream.h>
#include <redis-cpp/execute.h>
#include <string>

static void BM_RedisCpp_Lpush(benchmark::State& state) {
    std::string host = std::getenv("REDIS_HOST") ? std::getenv("REDIS_HOST") : "127.0.0.1";
    std::string port = std::getenv("REDIS_PORT") ? std::getenv("REDIS_PORT") : "6379";

    try {
        std::shared_ptr<std::iostream> stream = rediscpp::make_stream(host, port);
        std::string key = "test_list";
        std::string value = "test_value";

        for (auto _ : state) {
            // Push a value to the left of the list
            auto response = rediscpp::execute(*stream, "LPUSH", key, value);
            // Ensure the response is processed to avoid optimization out
            benchmark::DoNotOptimize(response);
        }
        // Clean up the key after the benchmark
        rediscpp::execute(*stream, "DEL", key);
    } catch (const std::exception& e) {
        state.SkipWithError(e.what());
    }
}

BENCHMARK(BM_RedisCpp_Lpush);
