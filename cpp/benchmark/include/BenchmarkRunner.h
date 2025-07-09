#ifndef BENCHMARK_LIB_RUNNER_H
#define BENCHMARK_LIB_RUNNER_H

#include <functional>
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include "BenchmarkConfig.h"
#include "Statistics.h"
#include "CsvExporter.h"

namespace benchmark {

// Type alias for a benchmark function that takes a config and a statistics object
using BenchmarkFunction = std::function<void(const BenchmarkConfig&, Statistics&)>;

// Registry class to store benchmark functions
class BenchmarkRegistry {
public:
    // Get the singleton instance of the registry
    static BenchmarkRegistry& getInstance() {
        static BenchmarkRegistry instance;
        return instance;
    }

    // Register a benchmark function with a name
    void registerBenchmark(const std::string& name, BenchmarkFunction func) {
        benchmarks_[name] = func;
    }

    // Get all registered benchmarks
    const std::map<std::string, BenchmarkFunction>& getBenchmarks() const {
        return benchmarks_;
    }

private:
    BenchmarkRegistry() = default;
    std::map<std::string, BenchmarkFunction> benchmarks_;
};

} // namespace benchmark

// Macro to register a benchmark function
#define REGISTER_BENCHMARK(name, func) \
    namespace { \
        struct BenchmarkRegistrar_##name { \
            BenchmarkRegistrar_##name() { \
                benchmark::BenchmarkRegistry::getInstance().registerBenchmark(#name, func); \
            } \
        }; \
        static BenchmarkRegistrar_##name registrar; \
    }

// Macro to define a default main function for benchmark execution
#define BENCHMARK_MAIN(project_name) \
    int main(int argc, char* argv[]) { \
        size_t iterations = 100000; \
        std::string test_run = ""; \
        benchmark::CsvExporter::TimeUnit time_unit = benchmark::CsvExporter::NANOSECONDS; \
        for (int i = 1; i < argc; ++i) { \
            std::string arg = argv[i]; \
            if ((arg == "--iterations" || arg == "-i") && i + 1 < argc) { \
                iterations = std::stoul(argv[++i]); \
            } else if (arg == "--testrun" && i + 1 < argc) { \
                test_run = "_" + std::string(argv[++i]); \
            } else if (arg == "--unit" && i + 1 < argc) { \
                std::string unit_str = argv[++i]; \
                if (unit_str == "ns") { \
                    time_unit = benchmark::CsvExporter::NANOSECONDS; \
                } else if (unit_str == "us") { \
                    time_unit = benchmark::CsvExporter::MICROSECONDS; \
                } else if (unit_str == "ms") { \
                    time_unit = benchmark::CsvExporter::MILLISECONDS; \
                } else if (unit_str == "s") { \
                    time_unit = benchmark::CsvExporter::SECONDS; \
                } else { \
                    std::cerr << "Invalid time unit: " << unit_str << ". Using default (ns)." << std::endl; \
                } \
            } \
        } \
        benchmark::BenchmarkConfig config(iterations); \
        std::map<std::string, benchmark::Statistics> operation_stats; \
        const auto& benchmarks = benchmark::BenchmarkRegistry::getInstance().getBenchmarks(); \
        for (const auto& pair : benchmarks) { \
            const std::string& name = pair.first; \
            const benchmark::BenchmarkFunction& func = pair.second; \
            std::cout << "Running benchmark: " << name << "...\n"; \
            func(config, operation_stats[name]); \
        } \
        std::cout << "Exporting results to CSV...\n"; \
        benchmark::CsvExporter exporter(project_name, test_run, time_unit); \
        bool export_success = exporter.exportAllToCsv(operation_stats); \
        if (export_success) { \
            std::cout << "Results exported successfully to " << project_name << "_raw" << test_run << ".csv and " \
                      << project_name << "_stats" << test_run << ".csv\n"; \
        } else { \
            std::cerr << "Failed to export results to CSV.\n"; \
        } \
        return 0; \
    }


#endif // BENCHMARK_LIB_RUNNER_H
