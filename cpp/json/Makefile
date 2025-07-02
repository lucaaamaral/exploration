# Makefile to manage testing for nlohmann/json

# Configuration for multiple runs
RUNS ?= 10

.PHONY: all test clean prune help poc benchmark benchmark-multi

all: help

help:
	@echo "Usage: make [target]"
	@echo "Targets:"
	@echo "  test         - Run test for nlohmann/json"
	@echo "  benchmark    - Run benchmark for nlohmann/json"
	@echo "  benchmark-multi - Run benchmark multiple times and save results"
	@echo "  clean        - Clean up Docker containers and images for nlohmann/json"
	@echo "  prune        - Manually remove Docker containers with hash-only image names and unnamed images"
	@echo "  poc          - Run proof of concept for nlohmann/json"
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

test:
	@echo "Building Docker image for nlohmann/json exploration..."
	@docker build --target json_example -t json-exploration .
	@$(MAKE) prune
	@echo "Running test for nlohmann/json..."
	@docker run json-exploration
	@echo "Test for nlohmann/json completed"

benchmark:
	@echo "Building Docker image for nlohmann/json benchmark..."
	@docker build --target json_benchmark -t json-benchmark .
	@$(MAKE) prune
	@echo "Running benchmark for nlohmann/json..."
	@docker run json-benchmark
	@echo "Benchmark for nlohmann/json completed"

# Target to run benchmarks multiple times and save results in CSV format
benchmark-multi:
	@echo "Building Docker image for nlohmann/json benchmark..."
	@docker build --target json_benchmark -t json-benchmark .
	@$(MAKE) prune
	@echo "Running benchmarks for nlohmann/json $(RUNS) times..."
	@mkdir -p results
	@for i in $$(seq 1 $(RUNS)); do \
		echo "Run $$i of $(RUNS)"; \
		RESULT_FILE="results/json_results_run_$$i.csv"; \
		docker run -v $$(pwd)/results:/results \
			json-benchmark \
			--benchmark_out_format=csv --benchmark_out=/$$RESULT_FILE; \
		if [ $$? -eq 0 ]; then \
			echo "Run $$i completed. Results saved to $$RESULT_FILE"; \
		else \
			echo "Run $$i failed. Check output for errors."; \
		fi \
	done
	@echo "All benchmark runs for nlohmann/json completed. Results are stored in the results directory."

poc:
	@echo "Building Docker image for nlohmann/json proof of concept..."
	@tar -czh . | docker build --target json_poc -t json-poc -
	@$(MAKE) prune
	@echo "Running proof of concept for nlohmann/json..."
	@docker run --network redis-network -v $$(pwd)/poc/demo_files:/app/poc/demo_files -e REDIS_HOST=redis-server -e REDIS_PORT=6379 json-poc
	@echo "Proof of concept for nlohmann/json completed"

clean:
	@echo "Cleaning up nlohmann/json test containers..."
	@if docker ps -a | grep -q json-; then \
		CONTAINERS=`docker ps -a | grep json- | awk '{print $$1}'`; \
		docker rm $$CONTAINERS; \
	fi
	@echo "Cleaning up nlohmann/json test images..."
	@if docker images | grep -q json-; then \
		IMAGES=`docker images | grep json- | awk '{print $$3}' | sort -u`; \
		docker rmi $$IMAGES; \
	fi
	@echo "Cleanup complete for nlohmann/json"
