# Makefile to manage the Redis Docker container for performance testing

CONTAINER_NAME := redis-server
NETWORK_NAME := redis-network
PORT := 6379

.PHONY: all build start stop clean help

all: help

help:
	@echo "Usage: make [target]"
	@echo "Targets:"
	@echo "  build    - Build the Redis Docker image"
	@echo "  start    - Start the Redis container"
	@echo "  stop     - Stop the Redis container"
	@echo "  clean    - Remove the Redis container, image, and network"
	@echo "  help     - Show this help message"

build:
	@echo "No build step needed. Using official redis:8.0-alpine image directly."

start:
	@echo "Creating Docker network if it doesn't exist..."
	@if ! docker network ls | grep -q $(NETWORK_NAME); then \
		docker network create $(NETWORK_NAME); \
		echo "Network $(NETWORK_NAME) created"; \
	else \
		echo "Network $(NETWORK_NAME) already exists"; \
	fi
	@echo "Starting Redis container..."
	@if docker ps -a | grep -q $(CONTAINER_NAME); then \
		echo "Container $(CONTAINER_NAME) already exists, removing it..."; \
		docker rm -f $(CONTAINER_NAME); \
	fi

	docker run -d --name $(CONTAINER_NAME) \
		--network $(NETWORK_NAME) \
		-p $(PORT):6379 \
		-v $(PWD)/redis.conf:/usr/local/etc/redis/redis.conf \
		redis:8.0-alpine \
		$(CONTAINER_NAME) /usr/local/etc/redis/redis.conf

	@echo "Redis container started as $(CONTAINER_NAME) on port $(PORT)"
	@echo "Use 'docker ps' to verify it's running"

stop:
	@echo "Stopping Redis container..."
	@if docker ps | grep -q $(CONTAINER_NAME); then \
		docker stop $(CONTAINER_NAME); \
		echo "Redis container $(CONTAINER_NAME) stopped"; \
	else \
		echo "Redis container $(CONTAINER_NAME) is not running"; \
	fi

clean:
	@echo "Cleaning up Redis container..."
	@if docker ps -a | grep -q $(CONTAINER_NAME); then \
		docker rm -f $(CONTAINER_NAME); \
		echo "Redis container $(CONTAINER_NAME) removed"; \
	fi
	@if docker network ls | grep -q $(NETWORK_NAME); then \
		docker network rm $(NETWORK_NAME); \
		echo "Network $(NETWORK_NAME) removed"; \
	fi
	@echo "Cleanup complete"
