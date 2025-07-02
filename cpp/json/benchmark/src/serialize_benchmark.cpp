#include <benchmark/benchmark.h>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

// Small JSON object
static json small_json_obj() {
    json j;
    j["name"] = "John Doe";
    j["age"] = 30;
    j["city"] = "New York";
    return j;
}

// Medium JSON object (nested structure)
static json medium_json_obj() {
    json j;
    j["person"]["name"] = "John Doe";
    j["person"]["age"] = 30;
    j["person"]["address"]["street"] = "123 Main St";
    j["person"]["address"]["city"] = "New York";
    j["person"]["address"]["zip"] = "10001";
    j["person"]["address"]["country"] = "USA";
    j["person"]["contacts"][0]["type"] = "email";
    j["person"]["contacts"][0]["value"] = "john@example.com";
    j["person"]["contacts"][1]["type"] = "phone";
    j["person"]["contacts"][1]["value"] = "+1-555-123-4567";
    return j;
}

// Large JSON object (array of objects)
static json large_json_obj() {
    json j;
    for (int i = 0; i < 100; ++i) {
        j["data"][i]["id"] = i;
        j["data"][i]["name"] = "Item " + std::to_string(i);
        j["data"][i]["value"] = i * 10.5;
    }
    return j;
}

static void BM_SerializeSmallJson(benchmark::State& state) {
    json j = small_json_obj();
    for (auto _ : state) {
        std::string serialized = j.dump();
        benchmark::DoNotOptimize(serialized);
    }
}
BENCHMARK(BM_SerializeSmallJson);

static void BM_SerializeMediumJson(benchmark::State& state) {
    json j = medium_json_obj();
    for (auto _ : state) {
        std::string serialized = j.dump();
        benchmark::DoNotOptimize(serialized);
    }
}
BENCHMARK(BM_SerializeMediumJson);

static void BM_SerializeLargeJson(benchmark::State& state) {
    json j = large_json_obj();
    for (auto _ : state) {
        std::string serialized = j.dump();
        benchmark::DoNotOptimize(serialized);
    }
}
BENCHMARK(BM_SerializeLargeJson);
