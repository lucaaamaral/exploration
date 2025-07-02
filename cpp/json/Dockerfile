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

# Copy the nlohmann/json library
COPY --link lib/json /app/lib/json

FROM base AS json_example

# Build the exploration code
COPY --link test /app/test
RUN mkdir -p /app/test/build && \
    cd /app/test/build && \
    cmake .. && \
    make -j$(nproc)

# Set the entrypoint to run the basic example
ENTRYPOINT ["/app/test/build/basic_example"]

FROM base AS json_benchmark

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
ENTRYPOINT ["/app/benchmark/build/json_benchmark"]

FROM base AS json-redis

RUN apk add --no-cache hiredis-dev

# Build the redis-plus-plus library
COPY --link lib/redis-plus-plus /app/lib/redis-plus-plus
RUN mkdir -p /app/lib/redis-plus-plus/build && \
    cd /app/lib/redis-plus-plus/build && \
    cmake .. && \
    make -j$(nproc) && \
    make install

# Build and install the redis-extended library
COPY --link lib/redis-extended /app/lib/redis-extended
RUN mkdir -p /app/lib/redis-extended/build && \
    cd /app/lib/redis-extended/build && \
    cmake .. && \
    make -j$(nproc) && \
    make install

FROM json-redis AS json_poc

# Build the proof of concept code
COPY poc /app/poc
RUN mkdir -p /app/poc/build && \
    cd /app/poc/build && \
    cmake .. && \
    make -j$(nproc)

# Set the entrypoint to run the proof of concept
ENTRYPOINT ["/app/poc/build/json_poc"]
