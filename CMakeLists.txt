cmake_minimum_required(VERSION 3.15) 

project(opencv_benchmark )

# Forbid in-source build
if("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
    message(
        FATAL_ERROR
	"In source build not permitted"
    )
endif()

# ------------------------------------------------------------------------------
# Benchmarks
# ------------------------------------------------------------------------------
set(BENCH_DIR "${CMAKE_SOURCE_DIR}/src")
set(BENCHMARK_DOWNLOAD_DEPENDENCIES ON)
add_subdirectory(${BENCH_DIR} )

############ 	


