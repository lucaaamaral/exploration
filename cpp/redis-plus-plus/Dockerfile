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
COPY test /app/test
RUN mkdir -p /app/test/build && \
    cd /app/test/build && \
    cmake .. && \
    make -j$(nproc)

# Set the entrypoint to run the basic example
ENTRYPOINT ["/app/test/build/basic_example"]

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

FROM redis_plus_plus AS poc

# Build the proof of concept code for key-value operations
COPY poc /app/poc
RUN mkdir -p /app/poc/build && \
    cd /app/poc/build && \
    cmake .. && \
    make -j$(nproc)

# Set the entrypoint to run the proof of concept for key-value operations
ENTRYPOINT ["/app/poc/build/poc_main"]

FROM redis_plus_plus AS redis-extended
# Build and install the redis-extended library
COPY --link lib/redis-extended /app/lib/redis-extended
RUN mkdir -p /app/lib/redis-extended/build && \
    cd /app/lib/redis-extended/build && \
    cmake .. && \
    make -j$(nproc) && \
    make install

FROM redis-extended AS poc-pubsub

# Install nlohmann/json for JSON processing
RUN git clone https://github.com/nlohmann/json.git /tmp/json \
    && mkdir -p /tmp/json/build \
    && cd /tmp/json/build \
    && cmake .. \
    && make -j$(nproc) \
    && make install \
    && cd / \
    && rm -rf /tmp/json

# Build the proof of concept code for pub/sub operations
COPY poc-pub-sub /app/poc-pub-sub
RUN mkdir -p /app/poc-pub-sub/build && \
    cd /app/poc-pub-sub/build && \
    cmake .. && \
    make -j$(nproc)

# Set the entrypoint to run the proof of concept for pub/sub operations
ENTRYPOINT ["/app/poc-pub-sub/build/pubsub_poc"]
