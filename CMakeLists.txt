cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_COMPILER "/usr/bin/g++-10" CACHE STRING "g++ compiler" FORCE)
set(CMAKE_C_COMPILER "/usr/bin/gcc-10" CACHE STRING "gcc compiler" FORCE)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(GRID LANGUAGES C CXX)

find_package(TBB REQUIRED)

# add test executables
file(GLOB TEST_SOURCES testsuite/*.cpp)
foreach(TEST_SOURCE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)

    add_executable(${TEST_NAME} ${TEST_SOURCE})
    target_include_directories(${TEST_NAME} PRIVATE include)
    target_link_libraries(${TEST_NAME} 
        ${TBB_IMPORTED_TARGETS})
    target_compile_options(${TEST_NAME} PRIVATE
            -g -O3 -Wfatal-errors)
endforeach()
