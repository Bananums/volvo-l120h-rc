cmake_minimum_required(VERSION 3.16)

project(volvo-l120h-host)

set(CMAKE_CXX_STANDARD 17)
if(NOT CMAKE_CXX_STANDARD OR CMAKE_CXX_STANDARD LESS 17)
    message(FATAL_ERROR "C++17 or later is required. Recommended is C++17")
endif()

add_executable(main src/main.cc)