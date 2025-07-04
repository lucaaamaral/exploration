# Task: Custom Benchmark-Time Framework for Redis-Plus-Plus

## Objective
Develop a custom benchmarking framework to measure the time taken for publish and subscribe operations in redis-plus-plus, store the deltas in a data structure, compute statistics (mean, median, P90, standard deviation), export results to CSV, and ensure modularity for future test additions.

## Detailed Plan for Benchmark-Time Implementation

1. **Create Directory Structure**:
   - Establish a new `benchmark-time` directory under `cpp/redis-plus-plus/` to house the custom timing benchmarks.
   - Subdirectories will include `src/` for source files, `include/` for headers, and `results/` for output CSV files.
   - This separation ensures modularity and keeps the new implementation distinct from existing benchmark code.

2. **Develop Timing Mechanism**:
   - Implement a custom timing framework using C++ high-resolution timers (e.g., `std::chrono::high_resolution_clock`) to capture precise durations for each publish and subscribe operation.
   - Measure the time delta for each atomic operation (e.g., time from publish call to completion, time from publish to message receipt for subscribe).
   - Avoid reliance on Google Benchmark's timing controls to prevent compatibility issues encountered previously.

3. **Data Structure for Deltas**:
   - Use a `std::vector<double>` to store time deltas (in milliseconds or microseconds) for each operation run.
   - Maintain separate vectors for publish and subscribe operations to ensure data separation and clarity in analysis.
   - This structure allows for easy iteration over data for statistical computations, is efficient for storage and access, and supports scalability for varying numbers of iterations.

4. **Statistical Analysis**:
   - Implement functions to calculate:
     - **Mean**: Average time taken across all operations.
     - **Median**: Middle value of sorted deltas to represent typical performance.
     - **P90 (90th Percentile)**: Time below which 90% of operations complete, indicating performance under higher load.
     - **Standard Deviation**: Measure of variability in operation times to assess consistency.
   - Sort the data as needed for median and percentile calculations using standard algorithms (e.g., `std::sort`).

5. **CSV Export**:
   - Develop a function or class named `CsvExporter` to handle the generation of CSV files, ensuring modularity and reusability.
   - Take input as collected time deltas from `std::vector<double>` for each operation type and computed statistics (mean, median, P90, standard deviation) from a `Statistics` module.
   - **File Structure**:
     - Generate separate CSV files for publish and subscribe operations, or a single file with clearly labeled sections, configurable via command-line arguments or configuration.
     - Include a raw data section listing all deltas under headers like "Operation,Delta (ms)" for a complete record.
     - Append a summary section with computed statistics under headers such as "Mean (ms),Median (ms),P90 (ms),StdDev (ms)" for a quick performance overview.
   - **Naming Convention**:
     - Name files as `PROJECTNAME_TESTRUN.csv`, where `PROJECTNAME` is derived from the CMake project configuration (e.g., via `PROJECT_NAME`), and `TESTRUN` is obtained from the `TESTRUN` environment variable with a fallback to "default_run" if not set.
     - Append operation identifiers if needed (e.g., `PROJECTNAME_TESTRUN_publish.csv`, `PROJECTNAME_TESTRUN_subscribe.csv`) to differentiate results.
   - **Formatting and Writing**:
     - Format CSV as plain text with comma-separated values for compatibility with analysis tools.
     - Include headers at the top of each section or file for clarity.
     - Write files to a `results/` subdirectory within `benchmark-time/` using standard C++ file I/O (e.g., `std::ofstream`).
   - **Configurability and Error Handling**:
     - Make output directory and file naming conventions configurable.
     - Implement error handling for file operations (e.g., directory creation, write failures) to ensure robustness.

6. **Modular Design**:
   - Structure the code with reusable components:
     - A `Timer` class or function for capturing deltas.
     - A `Statistics` class or module for computing and storing stats.
     - A `CsvExporter` class or function for file output.
     - Operation-specific modules (e.g., `PublishBenchmark`, `SubscribeBenchmark`) that can be extended for new tests.
   - Use configuration files or command-line arguments to specify parameters like number of iterations, Redis connection details, and output paths, enhancing flexibility.
   - Ensure new tests can be added by creating new operation classes or functions that integrate with the existing timing and reporting framework.

7. **Integration with Existing Setup**:
   - Update the existing `Makefile` in `cpp/redis-plus-plus/` to include a new target (e.g., `benchmark-time`) for building and running this custom timing benchmark.
   - Leverage the Docker setup used in other targets (e.g., `benchmark-multi`) for consistency in environment and Redis server access.
   - Ensure the new benchmark can run independently or alongside existing benchmarks, maintaining compatibility with current build processes.

## Additional Considerations
- The benchmark will default to a significant number of iterations (e.g., 10000 operations per run) to capture reliable performance data, adjustable via configuration.
- Error handling will be implemented to manage Redis connection issues or operation failures, logging errors without disrupting the benchmark flow.
- Documentation within the code and in this `TASK.md` will detail how to extend the framework with new operations or modify existing parameters.

## Next Steps
- Create the `benchmark-time` directory structure and necessary subdirectories.
- Develop source files and headers for timing, statistics, and CSV export functionalities.
- Implement publish and subscribe operation benchmarks using the custom timing framework.
- Update the `Makefile` to include a new target for building and running the `benchmark-time` suite.
- Test the implementation to ensure accurate timing, correct statistical computations, and proper CSV output.
- Document usage instructions and extension guidelines for adding new tests.

This plan ensures a robust, modular, and extensible benchmarking framework for evaluating redis-plus-plus performance, focusing on precise timing of atomic operations and detailed statistical reporting.
