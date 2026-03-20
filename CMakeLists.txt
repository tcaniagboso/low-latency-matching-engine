cmake_minimum_required(VERSION 3.28)
project(MatchingEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# =========================
# Core Library
# =========================
add_library(engine_core
        order_book/deque_order_book.cpp
        order_book/list_order_book.cpp
        gateway/parser.cpp
)

target_include_directories(engine_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(engine_core PRIVATE -Wall -Wextra -Wpedantic)

# =========================
# Main Executable
# =========================
add_executable(matching_engine main.cpp)

target_link_libraries(matching_engine PRIVATE engine_core)

# =========================
# Testing (future)
# =========================
enable_testing()