cmake_minimum_required (VERSION 3.9)

set(VERSION "0.0.1")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(CXXFeatureCheck)
cxx_feature_check(STEADY_CLOCK)

file(GLOB TEST_FILES Log.h)
include_directories(${PROJECT_SOURCE_DIR}/lib)
add_executable(etz-test Main.cpp)
