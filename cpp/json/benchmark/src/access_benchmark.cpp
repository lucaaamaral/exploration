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

static void BM_AccessSmallJson(benchmark::State& state) {
    json j = small_json_obj();
    for (auto _ : state) {
        std::string name = j["name"].get<std::string>();
        int age = j["age"].get<int>();
        std::string city = j["city"].get<std::string>();
        benchmark::DoNotOptimize(name);
        benchmark::DoNotOptimize(age);
        benchmark::DoNotOptimize(city);
    }
}
BENCHMARK(BM_AccessSmallJson);

static void BM_AccessMediumJson(benchmark::State& state) {
    json j = medium_json_obj();
    for (auto _ : state) {
        std::string name = j["person"]["name"].get<std::string>();
        std::string city = j["person"]["address"]["city"].get<std::string>();
        std::string email = j["person"]["contacts"][0]["value"].get<std::string>();
        benchmark::DoNotOptimize(name);
        benchmark::DoNotOptimize(city);
        benchmark::DoNotOptimize(email);
    }
}
BENCHMARK(BM_AccessMediumJson);

static void BM_AccessLargeJson(benchmark::State& state) {
    json j = large_json_obj();
    for (auto _ : state) {
        double value = 0.0;
        for (int i = 0; i < 100; ++i) {
            value += j["data"][i]["value"].get<double>();
        }
        benchmark::DoNotOptimize(value);
    }
}
BENCHMARK(BM_AccessLargeJson);

static void BM_ModifySmallJson(benchmark::State& state) {
    for (auto _ : state) {
        json j = small_json_obj();
        j["age"] = 31;
        j["city"] = "Los Angeles";
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(BM_ModifySmallJson);

static void BM_ModifyMediumJson(benchmark::State& state) {
    for (auto _ : state) {
        json j = medium_json_obj();
        j["person"]["age"] = 31;
        j["person"]["address"]["city"] = "Los Angeles";
        j["person"]["contacts"][1]["value"] = "+1-555-987-6543";
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(BM_ModifyMediumJson);

static void BM_ModifyLargeJson(benchmark::State& state) {
    for (auto _ : state) {
        json j = large_json_obj();
        for (int i = 0; i < 100; ++i) {
            j["data"][i]["value"] = i * 20.5;
        }
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(BM_ModifyLargeJson);
