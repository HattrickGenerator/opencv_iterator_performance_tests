set(BENCH_LIBS_DIR "${CMAKE_SOURCE_DIR}/src/benchmark/lib")

set(GBENCH_RECIPE_DIR googlebench-recipe)
set(GBENCH_SOURCE_DIR googlebench-src)

################################# BENCHMARKS ########################################
#Set up Google benchmark installation thanks to 
#Thanks to: https://chromium.googlesource.com/external/github.com/google/googletest/+/HEAD/googletest/README.md

# Download and unpack google benchmark at configure time
if (NOT EXISTS ${BENCH_LIBS_DIR}/${GBENCH_RECIPE_DIR}/CMakeLists.txt)
    configure_file(${BENCH_DIR}/CMakeLists.txt.gbench ${BENCH_LIBS_DIR}/${GBENCH_RECIPE_DIR}/CMakeLists.txt)
    execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
        WORKING_DIRECTORY "${BENCH_LIBS_DIR}/${GBENCH_RECIPE_DIR}"
        RESULT_VARIABLE result)
    if(result)
        message(FATAL_ERROR "CMake step for google benchmark failed: ${result}")
    endif()
    execute_process(COMMAND "${CMAKE_COMMAND}" --build . WORKING_DIRECTORY "${BENCH_LIBS_DIR}/${GBENCH_RECIPE_DIR}")
endif()



add_subdirectory("${BENCH_LIBS_DIR}/${GBENCH_SOURCE_DIR}")
add_subdirectory(${BENCH_DIR})
