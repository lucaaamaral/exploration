#ifndef BENCHMARK_TIME_CONFIG_H
#define BENCHMARK_TIME_CONFIG_H

#include <cstddef>

struct BenchmarkConfig {
    size_t iterations;
    // Additional configuration parameters can be added here as needed
    BenchmarkConfig(size_t iter = 10000) : iterations(iter) {}
};

#endif // BENCHMARK_TIME_CONFIG_H
