set(BENCHMARKS benchmark_iterator)

set(SOURCES
    test-main.cc
    algorithm.hh
    pointer-tuple-replacer.hh
    variadic-continuous-checker.hh
)

find_package(OpenCV REQUIRED core)

add_executable(${BENCHMARKS} ${SOURCES})

target_link_libraries(${BENCHMARKS} benchmark::benchmark gtest gtest_main  opencv_core)

target_include_directories(${BENCHMARKS} PRIVATE "${CMAKE_SOURCE_DIR}/opencv/modules/core/include" ${CMAKE_BINARY_DIR})
set_target_properties(${BENCHMARKS} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

