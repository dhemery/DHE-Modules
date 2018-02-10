cmake_minimum_required(VERSION 3.9)
project(DHE_Modules DESCRIPTION "DHE Modules for VCV Rack")

set(CMAKE_OSX_DEPLOYMENT_TARGET 10.7)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_SHARED_LINKER_FLAGS "-undefined dynamic_lookup -shared")

add_library(rack INTERFACE)
target_include_directories(rack
        INTERFACE
        ../../include
        ../../dep/include
        )

add_library(modules
        src/modules/stage.cpp
        src/modules/stage.h
        src/modules/upstage.h
        )
target_include_directories(modules PUBLIC src)
target_link_libraries(modules PUBLIC rack)

add_library(plugin SHARED
        src/gui/stage-widget.cpp
        src/gui/stage-widget.h
        src/gui/upstage-widget.cpp
        src/gui/upstage-widget.h
        src/gui/widget.cpp
        src/gui/widget.h
        src/plugin/dhe-modules.cpp
        src/plugin/dhe-modules.h
        )
target_include_directories(plugin PUBLIC src)
target_link_libraries(plugin PUBLIC rack modules)

add_library(catch INTERFACE)
target_include_directories(catch INTERFACE lib/catch)

add_executable(tests
        test/runner/main.cpp
        test/follower-tests.cpp
        test/stage-tests.cpp
        test/upstage-tests.cpp
        )
target_link_libraries(tests modules catch)
