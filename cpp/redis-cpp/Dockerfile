# Use Alpine Linux as a lightweight base image
FROM alpine:3.22 AS base

# Install necessary build tools and dependencies
RUN apk add --no-cache \
    build-base \
    cmake \
    git \
    linux-headers \
    boost-dev \
    boost-thread \
    boost-system \
    boost-iostreams

# Set working directory
WORKDIR /app

FROM base AS redis_cpp

# Build the redis-cpp library
COPY lib/redis-cpp /app/lib/redis-cpp
RUN mkdir -p /app/lib/redis-cpp/build && \
    cd /app/lib/redis-cpp/build && \
    cmake .. && \
    make -j$(nproc) && \
    make install

FROM redis_cpp AS example

# Build the exploration code
COPY src /app/src
COPY CMakeLists.txt /app/
RUN mkdir -p /app/build && \
    cd /app/build && \
    cmake .. && \
    make -j$(nproc)

# Set the entrypoint to run the basic example
# Note: This assumes a Redis server is running and accessible
ENTRYPOINT ["/app/build/basic_example"]

FROM redis_cpp AS benchmark

# Install Google Benchmark
RUN git clone https://github.com/google/benchmark.git /tmp/benchmark \
    && mkdir -p /tmp/benchmark/build \
    && cd /tmp/benchmark/build \
    && cmake .. -DBENCHMARK_ENABLE_GTEST_TESTS=OFF \
    && make -j$(nproc) \
    && make install \
    && cd / \
    && rm -rf /tmp/benchmark

# Build the benchmark suite
COPY benchmark /app/benchmark
RUN mkdir -p /app/benchmark/build && \
    cd /app/benchmark/build && \
    cmake .. && \
    make -j$(nproc)

# Set the entrypoint to run the benchmarks
ENTRYPOINT ["/app/benchmark/build/redis_cpp_benchmark"]
