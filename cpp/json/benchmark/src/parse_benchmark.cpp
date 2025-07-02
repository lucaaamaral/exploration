#include <benchmark/benchmark.h>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

// Small JSON string
static const std::string small_json = R"({
    "name": "John Doe",
    "age": 30,
    "city": "New York"
})";

// Medium JSON string (nested structure)
static const std::string medium_json = R"({
    "person": {
        "name": "John Doe",
        "age": 30,
        "address": {
            "street": "123 Main St",
            "city": "New York",
            "zip": "10001",
            "country": "USA"
        },
        "contacts": [
            {"type": "email", "value": "john@example.com"},
            {"type": "phone", "value": "+1-555-123-4567"}
        ]
    }
})";

// Large JSON string (array of objects)
static std::string large_json() {
    std::string result = R"({"data": [)";
    for (int i = 0; i < 100; ++i) {
        result += R"({"id": )" + std::to_string(i) + R"(, "name": "Item )" + std::to_string(i) + R"(", "value": )" + std::to_string(i * 10.5) + R"(})";
        if (i < 99) result += ",";
    }
    result += R"(]})";
    return result;
}

static void BM_ParseSmallJson(benchmark::State& state) {
    for (auto _ : state) {
        json j = json::parse(small_json);
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(BM_ParseSmallJson);

static void BM_ParseMediumJson(benchmark::State& state) {
    for (auto _ : state) {
        json j = json::parse(medium_json);
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(BM_ParseMediumJson);

static void BM_ParseLargeJson(benchmark::State& state) {
    std::string large = large_json();
    for (auto _ : state) {
        json j = json::parse(large);
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(BM_ParseLargeJson);

// Provide the main function for Google Benchmark
BENCHMARK_MAIN();
