set(BENCHMARKS benchmarks)

add_subdirectory(benchmark)

set(SOURCES
    test-main.cc
)

add_executable(${BENCHMARKS} ${SOURCES})

target_link_libraries(${BENCHMARKS} benchmark::benchmark  opencv_core)

target_include_directories(${BENCHMARKS} PRIVATE "${CMAKE_SOURCE_DIR}/opencv/modules/core/include" ${CMAKE_BINARY_DIR})
set_target_properties(${BENCHMARKS} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

