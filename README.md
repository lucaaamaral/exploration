# Exploration

This repository serves as a collection of exploration code for library implementation and proof of concepts. The primary focus is on C++ implementations, with the potential for extensions to other languages and technologies. Additionally, it includes supporting tools developed in Python.

## Purpose

The goal of this repository is to experiment with and demonstrate various library functionalities and concepts through practical implementations. It acts as a sandbox for testing ideas, benchmarking performance, and validating approaches before integrating them into larger projects.

## Technologies Used

- **C++**: The core language for library implementations and proof of concepts.
- **Python**: Used for developing supporting tools, such as benchmarking scripts and configuration utilities.

## Repository Structure

- **cpp/**: Contains C++ projects focused on specific libraries and concepts.
  - **json/**: Exploration of the [nlohmann/json](https://github.com/nlohmann/json) library for JSON parsing and serialization, including benchmark suites, proof of concept code for Redis integration, and qualitative analysis.
  - **redis-cpp/**: Benchmarking and comparison of the [redis-cpp](https://github.com/tdv/redis-cpp) library for Redis operations in C++17, evaluating performance and usability against redis-plus-plus.
  - **redis-plus-plus/**: Benchmarking and comparison of the [redis-plus-plus](https://github.com/sewenew/redis-plus-plus) library for Redis operations in C++17, assessing performance and features against redis-cpp.
  - **interval_map/**: Implementation task for a custom `interval_map` data structure, focusing on efficient interval-based key-value associations in C++.

- **tools/**: Python-based utilities to support development and testing.
  - **benchmark/**: Scripts for calculating averages and other metrics from benchmark results.
  - **redis/**: Configuration files and tools for Redis setup and interaction.

## Building and Running

Projects in this repository primarily use **CMake** as the build system. However, compilation is intended to be triggered using **Make** within Docker containers to ensure consistent environments and dependencies. Dockerfiles are provided in relevant subdirectories to facilitate this process.

To build and run a specific project:
1. Navigate to the project directory (e.g., `cpp/json/`).
2. Use the provided Dockerfile to build the Docker image.
3. Run `make` inside the Docker container to compile the code using CMake configurations.

Detailed instructions may be found in the README files of individual subdirectories.

## License

This repository is licensed under the MIT License. See the [LICENSE](LICENSE) file for more information.

## Contributing

Contributions are welcome! If you have ideas for new explorations or improvements to existing code, please open an issue or submit a pull request.
