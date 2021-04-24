set(BENCHMARKS benchmarks)


set(SOURCES
    test-main.cc
)

add_executable(${BENCHMARKS} ${SOURCES})

target_link_libraries(${BENCHMARKS} )
set_target_properties(${BENCHMARKS} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

