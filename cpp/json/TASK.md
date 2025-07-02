# Task: Implement Redis Abstraction Class

## Overview
This task involves implementing a Redis abstraction class named `RedisKeyManager` to handle operations for a single Redis key, including lock, unlock, is locked, read, write, and update functionalities. The implementation will be done in a new directory within the `redis-plus-plus` project structure, with symlinks to make it accessible to the `json` project. A modular logging system will be designed to allow for future flexibility.

## Plan

1. **Create a New Directory for Redis Abstractions**:
   - Establish a new directory named `redis-extended` under `cpp/redis-plus-plus/lib/` to house the new Redis abstraction classes and operations. This keeps the abstractions separate from the core `redis-plus-plus` library while still being part of the broader Redis ecosystem in the project structure.

2. **Create Symlinks for Library Access**:
   - Create a symlink from `cpp/redis-plus-plus/lib/redis-plus-plus` to `cpp/json/lib/redis-plus-plus` to allow the `json` project to access the existing Redis library without duplication.
   - Create a symlink from `cpp/redis-plus-plus/lib/redis-extended` to `cpp/json/lib/redis-extended` to provide access to the new abstractions for the `json` project, ensuring consistency across projects without duplicating code.

3. **Implement Redis Abstractions in `cpp/redis-plus-plus/lib/redis-extended`**:
   - Define a header file `RedisKeyManager.h` in `cpp/redis-plus-plus/lib/redis-extended/` to declare a class named `RedisKeyManager`. This class will encapsulate operations for a single Redis key, including lock, unlock, is locked, read, write, and update functionalities.
   - Incorporate the locking logic directly from the existing `RedisLock` class (from `cpp/redis-plus-plus/poc/src/`) into `RedisKeyManager`, adapting methods like `tryLock()`, `unlock()`, and `isLocked()` for managing access to the key.
   - Implement data manipulation methods such as `read()`, `write()`, and `update()` to handle getting, setting, and modifying the value associated with the key, ensuring these operations are protected by the integrated locking mechanism.
   - Create a corresponding source file `RedisKeyManager.cpp` in the same directory to implement these methods, using the `sw::redis::Redis` interface for compatibility with the existing `redis-plus-plus` library.
   - Design a modular logging system by defining an abstract logging interface (e.g., `ILogger`) in a separate header file `Logging.h` within `cpp/redis-plus-plus/lib/redis-extended/`. Implement a basic default logger (e.g., `DefaultLogger`) that can be easily replaced with another logging library in the future. The `RedisKeyManager` class will use this logging interface to track operations and errors, ensuring flexibility for future changes.

4. **Update Build Configuration for `redis-plus-plus`**:
   - Modify or create a `CMakeLists.txt` in `cpp/redis-plus-plus/lib/redis-extended/` to build the new extended library, ensuring it links against the `redis-plus-plus` library.
   - Update the main `CMakeLists.txt` in `cpp/redis-plus-plus/lib/` if necessary to include `redis-extended` as a subproject or library component.

5. **Integrate with `json` Project**:
   - Update the `CMakeLists.txt` in `json/poc/` to reference the symlinked `redis-extended` library, ensuring the include paths point to the symlinked directories for header files.
   - Update `poc_main.cpp` or other relevant files in `json/poc/src/` to use the `RedisKeyManager` class for Redis operations, demonstrating its functionality with a single key as part of the proof of concept.

6. **Update Dockerfile for Symlink Handling**:
   - Adjust the `Dockerfile` in `cpp/json/` to handle symlinks by copying the actual content of the linked directories (`redis-plus-plus` and `redis-extended`) during the build process, or ensure the build context includes the necessary paths to resolve symlinks correctly.

7. **Testing and Validation**:
   - Ensure the Docker build and run process (as defined in the `Makefile` and `Dockerfile`) can compile and execute the updated code without issues, taking care of symlink resolution.
   - Test the implementation to confirm that the `RedisKeyManager` class correctly handles locking and data operations for a single key within the `json` POC environment, and verify that the modular logging system functions as expected.

## Status
- **Last Updated**: 7/1/2025
- **Current Step**: Completed testing and validation of `RedisKeyManager` class. The Docker build and run process executed successfully, and the POC demonstrated correct handling of locking and data operations for a single key, with the modular logging system functioning as expected. Task is now complete.
