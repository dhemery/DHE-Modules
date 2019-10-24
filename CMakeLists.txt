cmake_minimum_required(VERSION 3.12.4)
project(DHE-Modules VERSION 1.6.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

add_subdirectory(external)
add_subdirectory(plugin)
