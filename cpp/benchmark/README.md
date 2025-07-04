# Benchmark Library

A reusable C++ library for benchmarking operations, providing utilities for configuration, timing measurements, statistical analysis, and result exporting.

## Overview

This library offers a framework for creating and running benchmarks in C++ projects. It includes tools to configure benchmark parameters, measure execution times, compute statistical metrics, export results to CSV, and register custom benchmark functions with a default execution setup.

## Directory Structure

- `include/`: Header files for the library components.
- `src/`: Source files for the library (mostly placeholders as most implementations are inline in headers).
- `CMakeLists.txt`: CMake configuration to build the library.

## Components

- **BenchmarkConfig**: A struct to configure benchmark parameters like the number of iterations.
- **Statistics**: A class to measure and analyze execution times, providing metrics like mean, median, P90, and standard deviation.
- **CsvExporter**: A class to export benchmark results to CSV files in both raw data and summarized statistics formats.
- **BenchmarkRunner**: Provides a registry and macros to register benchmark functions and define a default `main` function for execution.

## Usage

### Building the Library

Use CMake to build the library as a static library:

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

This will create `libbenchmark.a` which can be linked against in other projects.

### Integrating into a Project

Add the library to your project's `CMakeLists.txt`:

```cmake
# Find the benchmark library (assuming it's installed or in a known path)
find_library(BENCHMARK_LIB benchmark HINTS path/to/lib)
include_directories(path/to/benchmark/include)

# Link against the library
target_link_libraries(your_target ${BENCHMARK_LIB})
```

### Creating a Benchmark

1. **Define Benchmark Functions**: Use the `REGISTER_BENCHMARK` macro to register your benchmark functions.

```cpp
#include <benchmark/BenchmarkRunner.h>

void myBenchmark(const benchmark::BenchmarkConfig& config, benchmark::Statistics& stats) {
    for (size_t i = 0; i < config.iterations; ++i) {
        stats.start_timer();
        // Your operation to benchmark
        stats.stop_timer();
    }
}

REGISTER_BENCHMARK(MyBenchmark, myBenchmark);
```

2. **Use Default Main Function**: Use the `BENCHMARK_MAIN` macro to define a default `main` function that runs all registered benchmarks.

```cpp
#include <benchmark/BenchmarkRunner.h>

BENCHMARK_MAIN("MyProject");
```

3. **Compile and Run**: Build your project with the benchmark library linked, and run the executable. Use command-line arguments to customize iterations, test run identifiers, and time units.

```bash
./my_benchmark --iterations 10000 --testrun test1 --unit us
```

### Output

Results are exported to CSV files in the `results/` directory by default:
- `MyProject_raw_test1.csv`: Raw timing data for each run.
- `MyProject_stats_test1.csv`: Statistical summary including mean, median, P90, standard deviation, and count.

## Customization

- **Custom Output Directory**: Use `CsvExporter::setOutputDir()` to change the output directory.
- **Custom Time Unit**: Use command-line arguments or `CsvExporter::setTimeUnit()` to change the time unit for results.

## License

This library is provided under the MIT License. See the main project LICENSE file for details.
