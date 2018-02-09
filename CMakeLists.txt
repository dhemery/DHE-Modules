cmake_minimum_required(VERSION 3.9)
project(DHE_Modules DESCRIPTION "DHE Modules for VCV Rack")

set(CMAKE_OSX_DEPLOYMENT_TARGET 10.7)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_SHARED_LINKER_FLAGS "-undefined dynamic_lookup -shared")

FILE(GLOB MODULE_SOURCES
        src/modules/*
        src/util/*)
FILE(GLOB GUI_SOURCES
        src/gui/*)
FILE(GLOB PLUGIN_SOURCES
        src/plugin/*)
FILE(GLOB TEST_SOURCES
        test/*)

include_directories(
        src
        ../../include
        ../../dep/include)
add_library(plugin SHARED
        ${MODULE_SOURCES}
        ${GUI_SOURCES}
        ${PLUGIN_SOURCES})

add_executable(tests
        ${TEST_SOURCES}
        ${MODULE_SOURCES})
target_link_libraries(tests)
