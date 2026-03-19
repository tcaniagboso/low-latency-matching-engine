cmake_minimum_required(VERSION 3.28)
project(MatchingEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# =========================
# Core Library
# =========================
add_library(engine_core
        order_book/simple_order_book.cpp
        engine/matching_engine.cpp
        gateway/parser.cpp
)

target_include_directories(engine_core PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/gateway
        ${CMAKE_CURRENT_SOURCE_DIR}/order_book
        ${CMAKE_CURRENT_SOURCE_DIR}/core
        ${CMAKE_CURRENT_SOURCE_DIR}/engine
)

# =========================
# Main Executable
# =========================
add_executable(matching_engine main.cpp)

target_link_libraries(matching_engine PRIVATE engine_core)