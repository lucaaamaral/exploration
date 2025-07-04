#ifndef BENCHMARK_LIB_CONFIG_H
#define BENCHMARK_LIB_CONFIG_H

#include <cstddef>

namespace benchmark {

struct BenchmarkConfig {
    size_t iterations;
    // Additional configuration parameters can be added here as needed
    BenchmarkConfig(size_t iter = 10000) : iterations(iter) {}
};

} // namespace benchmark

#endif // BENCHMARK_LIB_CONFIG_H
