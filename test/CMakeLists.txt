cmake_minimum_required (VERSION 3.8.0)

set(VERSION "0.0.1")

file(GLOB TEST_FILES Log.h)

include_directories(${PROJECT_SOURCE_DIR}/lib)
add_executable(etz-test Main.cpp)
