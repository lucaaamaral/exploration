#include <benchmark/benchmark.h>
#include <sw/redis++/redis++.h>
#include <string>
#include <vector>

static void BM_RedisPlusPlus_Smembers(benchmark::State& state) {
    std::string host = std::getenv("REDIS_HOST") ? std::getenv("REDIS_HOST") : "127.0.0.1";
    std::string port = std::getenv("REDIS_PORT") ? std::getenv("REDIS_PORT") : "6379";
    std::string connection = "tcp://" + host + ":" + port;

    try {
        sw::redis::Redis redis(connection);
        std::string key = "test_set";
        std::string member = "test_member";
        // Add some members to the set before the benchmark loop to ensure it has elements
        redis.sadd(key, member);
        redis.sadd(key, member + "1");
        redis.sadd(key, member + "2");

        for (auto _ : state) {
            // Get all members of the set
            std::vector<std::string> result;
            redis.smembers(key, std::back_inserter(result));
            // Ensure the response is processed to avoid optimization out
            benchmark::DoNotOptimize(result);
        }
        // Clean up the key after the benchmark
        redis.del(key);
    } catch (const std::exception& e) {
        state.SkipWithError(e.what());
    }
}

BENCHMARK(BM_RedisPlusPlus_Smembers);
