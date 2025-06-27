# Makefile to manage testing for redis-cpp

# Configuration variables for Redis connection
REDIS_HOST ?= redis-server
REDIS_PORT ?= 6379

.PHONY: all test start-redis stop-redis clean prune help

all: help

help:
	@echo "Usage: make [target]"
	@echo "Targets:"
	@echo "  test         - Run test for redis-cpp"
	@echo "  start-redis  - Start the Redis server container"
	@echo "  stop-redis   - Stop the Redis server container"
	@echo "  clean        - Clean up Docker containers and images for redis-cpp"
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
	@echo "Building Docker image for redis-cpp exploration..."
	@docker build --target example -t redis-cpp-exploration .
	@$(MAKE) prune
	@echo "Running test for redis-cpp..."
	@docker run --network redis-network \
		-e REDIS_HOST=$(REDIS_HOST) \
		-e REDIS_PORT=$(REDIS_PORT) \
		redis-cpp-exploration
	@echo "Test for redis-cpp completed"

clean:
	@echo "Cleaning up redis-cpp test containers..."
	@if docker ps -a | grep -q redis-cpp-exploration; then \
		CONTAINERS=`docker ps -a | grep redis-cpp-exploration | awk '{print $$1}'`; \
		docker rm $$CONTAINERS; \
	fi
	@echo "Cleaning up redis-cpp test image..."
	@if docker images | grep -q redis-cpp-exploration; then \
		docker rmi redis-cpp-exploration; \
	fi
	@echo "Cleanup debugging complete for redis-cpp"
