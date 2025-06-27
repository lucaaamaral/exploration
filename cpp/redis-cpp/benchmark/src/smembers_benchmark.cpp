#include <benchmark/benchmark.h>
#include <redis-cpp/stream.h>
#include <redis-cpp/execute.h>
#include <redis-cpp/value.h>
#include <string>
#include <vector>

static void BM_RedisCpp_Smembers(benchmark::State& state) {
    std::string host = std::getenv("REDIS_HOST") ? std::getenv("REDIS_HOST") : "127.0.0.1";
    std::string port = std::getenv("REDIS_PORT") ? std::getenv("REDIS_PORT") : "6379";

    try {
        std::shared_ptr<std::iostream> stream = rediscpp::make_stream(host, port);
        std::string key = "test_set";
        std::string member = "test_member";
        // Add some members to the set before the benchmark loop to ensure it has elements
        rediscpp::execute(*stream, "SADD", key, member);
        rediscpp::execute(*stream, "SADD", key, member + "1");
        rediscpp::execute(*stream, "SADD", key, member + "2");

        for (auto _ : state) {
            // Get all members of the set
            auto response = rediscpp::execute(*stream, "SMEMBERS", key);
            // Ensure the response is processed to avoid optimization out
            benchmark::DoNotOptimize(response);
            // Optionally, extract the array of members if needed for further processing
            if (response.is_array()) {
                std::vector<std::string> members;
                for (const auto& val : response.as_string_array()) {
                    members.push_back(val);
                }
                benchmark::DoNotOptimize(members);
            }
        }
        // Clean up the key after the benchmark
        rediscpp::execute(*stream, "DEL", key);
    } catch (const std::exception& e) {
        state.SkipWithError(e.what());
    }
}

BENCHMARK(BM_RedisCpp_Smembers);
