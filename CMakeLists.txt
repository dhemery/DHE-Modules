cmake_minimum_required(VERSION 3.9)
project(DHE_Modules DESCRIPTION "DHE Modules for VCV Rack")

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
set(CMAKE_SHARED_LINKER_FLAGS "-undefined dynamic_lookup -shared")
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.7)
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

include_directories(
        include
        ../../include
        ../../dep/include)

add_library(plugin SHARED
        src/DHEModules.cpp
        include/DHEModules.hpp
        src/FlipFlop.cpp
        include/FlipFlop.hpp
        src/Ramp.cpp
        include/Ramp.hpp
        src/Stage.cpp
        include/Stage.hpp
        src/StageWidget.cpp src/Freezer.cpp include/Freezer.hpp)
