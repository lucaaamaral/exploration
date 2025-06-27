# Makefile to manage testing for redis-plus-plus

# Configuration variables for Redis connection
REDIS_HOST ?= redis-server
REDIS_PORT ?= 6379

.PHONY: all test start-redis stop-redis clean prune help

all: help

help:
	@echo "Usage: make [target]"
	@echo "Targets:"
	@echo "  test         - Run test for redis-plus-plus"
	@echo "  start-redis  - Start the Redis server container"
	@echo "  stop-redis   - Stop the Redis server container"
	@echo "  clean        - Clean up Docker containers and images for redis-plus-plus"
	@echo "  prune        - Manually remove Docker containers with hash-only image names and unnamed images"
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

clean:
	@echo "Cleaning up redis-plus-plus test containers..."
	@if docker ps -a | grep -q redis-plus-plus-exploration; then \
		CONTAINERS=`docker ps -a | grep redis-plus-plus-exploration | awk '{print $$1}'`; \
		docker rm $$CONTAINERS; \
	fi
	@echo "Cleaning up redis-plus-plus test image..."
	@if docker images | grep -q redis-plus-plus-exploration; then \
		docker rmi redis-plus-plus-exploration; \
	fi
	@echo "Cleanup complete for redis-plus-plus"
