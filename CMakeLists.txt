cmake_minimum_required(VERSION 3.9)
project(DHE_Modules DESCRIPTION "DHE Modules for VCV Rack" VERSION 0.0.0)

set(CMAKE_OSX_DEPLOYMENT_TARGET 10.7)

add_library(rack INTERFACE)
target_include_directories(rack
        INTERFACE
        ../Rack/include
        ../Rack/dep/include
        )
target_compile_features(rack INTERFACE cxx_std_11)
target_compile_options(rack INTERFACE -stdlib=libc++)

add_library(modules STATIC
        src/modules/stage.cpp
        src/modules/stage.h
        src/modules/upstage.h
        src/util/d-flip-flop.h
        src/util/d-latch.h
        src/util/follower.h
        src/util/interval.h
        src/util/latch.h
        src/util/ramp.h
        src/util/sigmoid.h
        )
target_include_directories(modules PUBLIC src)
target_link_libraries(modules
        INTERFACE -stdlib=libc++
        PUBLIC rack
        )

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
target_link_libraries(plugin
        INTERFACE -stdlib=libc++
        PUBLIC modules rack
        PRIVATE "-undefined dynamic_lookup"
        )

add_executable(tests
        test/catch/catch.hpp
        test/runner/main.cpp
        test/follower-tests.cpp
        test/stage-tests.cpp
        test/upstage-tests.cpp
        )
target_include_directories(tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/test)
target_link_libraries(tests modules)
