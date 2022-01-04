cmake_minimum_required(VERSION 3.15) 

project(opencv_benchmark )
set(CMAKE_CXX_STANDARD 14)

# Forbid in-source build
if("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
    message(
        FATAL_ERROR
	"In source build not permitted"
    )
endif()

# ------------------------------------------------------------------------------
# Dependncies
set(GBENCH_DIR "${CMAKE_SOURCE_DIR}/benchmark")
set(OPENCV_DIR  "${CMAKE_SOURCE_DIR}/opencv")
if (NOT EXISTS ${GBENCH_DIR} OR NOT EXISTS ${OPENCV_DIR})
    message(
        FATAL_ERROR
	"Opencv or google benchmark missing. Maybe forgot to fetch git submodules with:
	git submodule init
	git submodule update
	?"
    )
endif()
# ------------------------------------------------------------------------------

# ------------------------------------------------------------------------------
# Benchmarks
# ------------------------------------------------------------------------------
set(BENCHMARK_DOWNLOAD_DEPENDENCIES ON)
add_subdirectory(${GBENCH_DIR})

set(BENCH_DIR "${CMAKE_SOURCE_DIR}/src")
add_subdirectory(${BENCH_DIR} )

set(BUILD_SHARED_LIBS OFF)
add_subdirectory(opencv)
############ 	


