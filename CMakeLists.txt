cmake_minimum_required(VERSION 3.10)

# set the project name
project(ResourceGuard)

message(STATUS "========= ${CMAKE_CXX_COMPILER_VERSION}")

option(BUILD_TEST "build tests" ON)

if(BUILD_TEST)
	enable_testing()
	add_subdirectory(test)
endif()