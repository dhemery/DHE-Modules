cmake_minimum_required(VERSION 3.9)
project(DHE_Modules DESCRIPTION "DHE Modules for VCV Rack" VERSION 0.0.0)

set(CMAKE_OSX_DEPLOYMENT_TARGET 10.7)
if (DEFINED ENV{RACK_DIR})
    set(RACK_DIR "$ENV{RACK_DIR}")
endif (DEFINED ENV{RACK_DIR})
if (NOT DEFINED RACK_DIR)
    set(RACK_DIR ../..)
endif (NOT DEFINED RACK_DIR)

add_library(rack INTERFACE)
target_include_directories(rack
        INTERFACE
        ${RACK_DIR}/include
        ${RACK_DIR}/dep/include
        )
target_compile_features(rack INTERFACE cxx_std_11)
target_compile_options(rack INTERFACE -stdlib=libc++)

add_library(plugin SHARED
        src/controllers/button-control.h
        src/controllers/duration-control.h
        src/controllers/input-port-control.h
        src/controllers/level-control.h
        src/controllers/modulated-control.h
        src/controllers/stage-controller.h
        src/controllers/swave-controller.h
        src/controllers/upstage-controller.h
        src/modules/booster-stage-module.h
        src/modules/booster-stage-widget.cpp
        src/modules/booster-stage-widget.h
        src/modules/swave-module.h
        src/modules/swave-widget.cpp
        src/modules/swave-module.h
        src/modules/stage-widget.cpp
        src/modules/stage-widget.h
        src/modules/upstage-module.h
        src/modules/upstage-widget.cpp
        src/modules/upstage-widget.h
        src/modules/module-widget.cpp
        src/modules/module-widget.h
        src/plugin/dhe-modules.cpp
        src/plugin/dhe-modules.h
        src/util/d-flip-flop.h
        src/util/d-latch.h
        src/util/follower.h
        src/util/interval.h
        src/util/latch.h
        src/util/ramp.h
        src/util/sigmoid.h
        )
target_link_libraries(plugin
        PUBLIC -stdlib=libc++
        PUBLIC rack
        PRIVATE "-undefined dynamic_lookup"
        )
target_include_directories(plugin PUBLIC src)

add_executable(tests
        src/controllers/upstage-controller.h
        src/controllers/stage-controller.h
        src/util/follower.h
        test/catch/catch.hpp
        test/runner/main.cpp
        test/follower-tests.cpp
        test/stage-tests.cpp
        test/upstage-tests.cpp
        )
target_compile_features(tests PUBLIC cxx_std_11)
target_compile_options(tests PUBLIC -stdlib=libc++)
target_include_directories(tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/test ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(tests PUBLIC -stdlib=libc++)