# Makefile to manage testing for redis-plus-plus

# Configuration variables for Redis connection
REDIS_HOST ?= redis-server
REDIS_PORT ?= 6379
# Configuration for multiple runs
RUNS ?= 10

.PHONY: all test start-redis stop-redis clean prune help poc

all: help

help:
	@echo "Usage: make [target]"
	@echo "Targets:"
	@echo "  test         - Run test for redis-plus-plus"
	@echo "  start-redis  - Start the Redis server container"
	@echo "  stop-redis   - Stop the Redis server container"
	@echo "  clean        - Clean up Docker containers and images for redis-plus-plus"
	@echo "  prune        - Manually remove Docker containers with hash-only image names and unnamed images"
	@echo "  poc          - Run proof of concept for redis-plus-plus (key-value operations)"
	@echo "  poc-pubsub   - Run proof of concept for redis-plus-plus (pub/sub operations)"
	@echo "  help         - Show this help message"

prune:
	@echo "Removing Docker containers with hash-only image names and unnamed images..."
	@CONTAINERS_TO_REMOVE=`docker ps -a --format '{{.ID}} {{.Image}}' | awk '$$2 ~ /^[0-9a-f]{12}$$/ {print $$1}'`; \
	if [ -n "$$CONTAINERS_TO_REMOVE" ]; then \
		docker rm -f $$CONTAINERS_TO_REMOVE > /dev/null; \
	fi
	@echo "Removing Docker unnamed images..."
	@IMAGES_TO_REMOVE=`docker images --format '{{.ID}} {{.Repository}}:{{.Tag}}' | grep -E '<none>|<none>|<none>:latest' | awk '{print $$1}' | sort -u`; \
	if [ -n "$$IMAGES_TO_REMOVE" ]; then \
		docker rmi -f $$IMAGES_TO_REMOVE > /dev/null; \
	fi

start-redis:
	@echo "Starting Redis server container..."
	@cd ../../tools/redis && make start
	@echo "Redis server started. Use 'docker ps' to verify it's running"

stop-redis:
	@echo "Stopping Redis server container..."
	@cd ../../tools/redis && make stop
	@echo "Redis server stopped"

test: start-redis
	@echo "Building Docker image for redis-plus-plus exploration..."
	@docker build --target example -t redis-plus-plus-exploration .
	@$(MAKE) prune
	@echo "Running test for redis-plus-plus..."
	@docker run --network redis-network \
		-e REDIS_HOST=$(REDIS_HOST) \
		-e REDIS_PORT=$(REDIS_PORT) \
		redis-plus-plus-exploration
	@echo "Test for redis-plus-plus completed"

benchmark: start-redis
	@echo "Building Docker image for redis-plus-plus benchmark..."
	@docker build --target benchmark -t redis-plus-plus-benchmark .
	@$(MAKE) prune
	@echo "Running benchmark for redis-plus-plus..."
	@docker run --network redis-network \
		-e REDIS_HOST=$(REDIS_HOST) \
		-e REDIS_PORT=$(REDIS_PORT) \
		redis-plus-plus-benchmark
	@echo "Benchmark for redis-plus-plus completed"

# Target to run benchmarks multiple times and save results in CSV format
benchmark-multi: start-redis
	@echo "Building Docker image for redis-plus-plus benchmark..."
	@docker build --target benchmark -t redis-plus-plus-benchmark .
	@$(MAKE) prune
	@echo "Running benchmarks for redis-plus-plus $(RUNS) times..."
	@mkdir -p results
	@for i in $$(seq 1 $(RUNS)); do \
		echo "Run $$i of $(RUNS)"; \
		RESULT_FILE="results/redis_plus_plus_results_run_$$i.csv"; \
		docker run --network redis-network -v $$(pwd)/results:/results \
			-e REDIS_HOST=$(REDIS_HOST) \
			-e REDIS_PORT=$(REDIS_PORT) \
			redis-plus-plus-benchmark \
			--benchmark_out_format=csv --benchmark_out=/$$RESULT_FILE; \
		if [ $$? -eq 0 ]; then \
			echo "Run $$i completed. Results saved to $$RESULT_FILE"; \
		else \
			echo "Run $$i failed. Check output for errors."; \
		fi \
	done
	@echo "All benchmark runs for redis-plus-plus completed. Results are stored in the results directory."

poc: start-redis
	@echo "Building Docker image for redis-plus-plus proof of concept (key-value operations)..."
	@docker build --target poc -t redis-plus-plus-poc .
	@$(MAKE) prune
	@echo "Running proof of concept for redis-plus-plus (key-value operations)..."
	@docker run --network redis-network \
		-e REDIS_HOST=$(REDIS_HOST) \
		-e REDIS_PORT=$(REDIS_PORT) \
		redis-plus-plus-poc
	@echo "Proof of concept for redis-plus-plus (key-value operations) completed"

poc-pubsub: start-redis
	@echo "Building Docker image for redis-plus-plus proof of concept (pub/sub operations)..."
	@docker build --target poc-pubsub -t redis-plus-plus-poc-pubsub .
	@$(MAKE) prune
	@echo "Running proof of concept for redis-plus-plus (pub/sub operations)..."
	@docker run --network redis-network \
		-e REDIS_HOST=$(REDIS_HOST) \
		-e REDIS_PORT=$(REDIS_PORT) \
		redis-plus-plus-poc-pubsub
	@echo "Proof of concept for redis-plus-plus (pub/sub operations) completed"

clean:
	@echo "Cleaning up redis-plus-plus test containers..."
	@if docker ps -a | grep -q redis-plus-plus; then \
		CONTAINERS=`docker ps -a | grep redis-plus-plus | awk '{print $$1}'`; \
		docker rm $$CONTAINERS; \
	fi
	@echo "Cleaning up redis-plus-plus test images..."
	@if docker images | grep -q redis-plus-plus; then \
		IMAGES=`docker images | grep redis-plus-plus | awk '{print $$3}' | sort -u`; \
		docker rmi $$IMAGES; \
	fi
	@echo "Cleanup complete for redis-plus-plus"
