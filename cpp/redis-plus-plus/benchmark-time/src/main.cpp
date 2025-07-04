#include <iostream>
#include <string>
#include <map>
#include <cstdlib>
#include "BenchmarkConfig.h"
#include "Statistics.h"
#include "CsvExporter.h"
#include "PublishBenchmark.h"
#include "PublishSizeBenchmark.h"
#include "SubscribeBenchmark.h"
#include "SubscribeSizeBenchmark.h"

int main(int argc, char* argv[]) {
    // Default configuration
    size_t iterations = 100000; // Default number of iterations
    std::string test_run = "";
    CsvExporter::TimeUnit time_unit = CsvExporter::NANOSECONDS; // Default time unit

    // Parse command line arguments if provided
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if ((arg == "--iterations" || arg == "-i") && i + 1 < argc) {
            iterations = std::stoul(argv[++i]);
        } else if (arg == "--testrun" && i + 1 < argc) {
            test_run = "_" + std::string(argv[++i]);
        } else if (arg == "--unit" && i + 1 < argc) {
            std::string unit_str = argv[++i];
            if (unit_str == "ns") {
                time_unit = CsvExporter::NANOSECONDS;
            } else if (unit_str == "us") {
                time_unit = CsvExporter::MICROSECONDS;
            } else if (unit_str == "ms") {
                time_unit = CsvExporter::MILLISECONDS;
            } else if (unit_str == "s") {
                time_unit = CsvExporter::SECONDS;
            } else {
                std::cerr << "Invalid time unit: " << unit_str << ". Using default (ns)." << std::endl;
            }
        }
    }

    // Set up benchmark configuration
    BenchmarkConfig config(iterations);

    // Prepare statistics collection for each operation
    std::map<std::string, Statistics> operation_stats;

    // Run Publish Benchmark (fixed size)
    std::cout << "Running Publish Benchmark (fixed size)...\n";
    PublishBenchmark pub_bench(config);
    pub_bench.run(operation_stats["publish"]);

    // Run Publish Benchmark (increasing size)
    std::cout << "Running Publish Benchmark (increasing size)...\n";
    PublishSizeBenchmark pub_size_bench(config);
    pub_size_bench.run(operation_stats["publish_size"]);

    // Run Subscribe Benchmark (fixed size)
    std::cout << "Running Subscribe Benchmark (fixed size)...\n";
    SubscribeBenchmark sub_bench(config);
    sub_bench.run(operation_stats["subscribe"]);

    // Run Subscribe Benchmark (increasing size)
    std::cout << "Running Subscribe Benchmark (increasing size)...\n";
    SubscribeSizeBenchmark sub_size_bench(config);
    sub_size_bench.run(operation_stats["subscribe_size"]);

    // Export results to CSV
    std::cout << "Exporting results to CSV...\n";
    CsvExporter exporter(PROJECT_NAME, test_run, time_unit);
    bool export_success = exporter.exportAllToCsv(operation_stats);
    if (export_success) {
        std::cout << "Results exported successfully to " << PROJECT_NAME << "_raw" << test_run << ".csv and "
                  << PROJECT_NAME << "_stats" << test_run << ".csv\n";
    } else {
        std::cerr << "Failed to export results to CSV.\n";
    }

    return 0;
}
