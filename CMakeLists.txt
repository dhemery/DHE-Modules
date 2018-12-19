cmake_minimum_required(VERSION 3.13)
project(DHE_Modules)

set(CMAKE_VERBOSE_MAKEFILE ON)

add_library(plugin SHARED)

target_sources(plugin
        PRIVATE
        include/display/controls.h
        include/display/panel.h
        include/util/duration.h
        include/util/gate.h
        include/util/mode.h
        include/util/phase-accumulator.h
        include/util/range.h
        include/util/rotation.h
        include/util/selector.h
        include/util/sigmoid.h
        include/util/signal.h
        include/util/stage-components.h
        include/util/trigger.h
        include/dhe-modules.h
        src/booster-stage.cpp
        src/cubic.cpp
        src/dhe-modules.cpp
        src/func.cpp
        src/hostage.cpp
        src/ranger.cpp
        src/stage.cpp
        src/swave.cpp
        src/tapers.cpp
        src/upstage.cpp
        src/xycloid.cpp)
set_target_properties(plugin
        PROPERTIES
        PREFIX "")
target_compile_definitions(plugin
        PRIVATE
        VERSION=0.6.4
        SLUG=DHE-Modules
        ARCH_MAC)
target_include_directories(plugin PRIVATE include)
target_link_libraries(plugin PRIVATE rack)
target_link_options(plugin
        PRIVATE
        "LINKER:-undefined,dynamic_lookup")

add_library(rack INTERFACE)
target_include_directories(rack
        INTERFACE
        ../Rack-SDK/include
        ../Rack-SDK/dep/include)
target_compile_features(rack INTERFACE
        cxx_std_11)

set(CMAKE_INSTALL_PREFIX /Users/Dale/programming/DHE-Modules/.cdev/plugins/DHE-Modules)
install(TARGETS plugin LIBRARY DESTINATION .)
install(DIRECTORY res DESTINATION .)
install(FILES LICENSE.txt DESTINATION .)
