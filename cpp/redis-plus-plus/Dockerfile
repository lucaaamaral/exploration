# Use Alpine Linux as a lightweight base image
FROM alpine:3.22 AS base

# Install necessary build tools and dependencies
RUN apk add --no-cache \
    build-base \
    cmake \
    git \
    linux-headers \
    hiredis-dev \
    boost-dev \
    boost-thread \
    boost-system \
    boost-iostreams

# Set working directory
WORKDIR /app

FROM base AS redis_plus_plus

# Build the redis-plus-plus library
COPY lib/redis-plus-plus /app/lib/redis-plus-plus
RUN mkdir -p /app/lib/redis-plus-plus/build && \
    cd /app/lib/redis-plus-plus/build && \
    cmake .. && \
    make -j$(nproc) && \
    make install

FROM redis_plus_plus AS example

# Build the exploration code
COPY src /app/src
COPY CMakeLists.txt /app/
RUN mkdir -p /app/build && \
    cd /app/build && \
    cmake .. && \
    make -j$(nproc)

# Set the entrypoint to run the basic example
ENTRYPOINT ["/app/build/basic_example"]

FROM redis_plus_plus AS benchmark

# Install Google Benchmark
RUN git clone https://github.com/google/benchmark.git /tmp/benchmark \
    && mkdir -p /tmp/benchmark/build \
    && cd /tmp/benchmark/build \
    && cmake .. -DBENCHMARK_ENABLE_GTEST_TESTS=OFF \
    && make -j$(nproc) \
    && make install \
    && cd / \
    && rm -rf /tmp/benchmark

# Build the benchmark suite in Release mode
COPY benchmark /app/benchmark
RUN mkdir -p /app/benchmark/build && \
    cd /app/benchmark/build && \
    cmake .. && \
    make -j$(nproc)

# Set the entrypoint to run the benchmarks
ENTRYPOINT ["/app/benchmark/build/redis_plus_plus_benchmark"]
