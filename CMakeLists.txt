cmake_minimum_required(VERSION 3.28)
project(High_Performance_Trade_Engine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(gateway order_book types)

add_executable(High_Performance_Trade_Engine
        main.cpp
        gateway/parser.cpp
)


