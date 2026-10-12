cmake_minimum_required(VERSION 3.20)

project(Project47 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(project47
    src/main.cpp
    src/scheduler.cpp
)

target_include_directories(project47 PRIVATE include)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(project47 PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()

enable_testing()

add_executable(test_scheduler
    tests/test_scheduler.cpp
    src/scheduler.cpp
    src/dfs_scheduler.cpp
)

target_include_directories(test_scheduler PRIVATE include)

add_test(NAME SchedulerTests COMMAND test_scheduler)

add_executable(benchmark_scheduler
    src/benchmark.cpp
    src/scheduler.cpp
    src/dfs_scheduler.cpp
)

target_include_directories(benchmark_scheduler PRIVATE include)