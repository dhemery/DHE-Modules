cmake_minimum_required(VERSION 3.13)
project(DHE-Modules
        VERSION 0.6.5
        LANGUAGES CXX)

add_library(components INTERFACE)
target_sources(components
        INTERFACE
        include/components/gate.h
        include/components/phase-accumulator.h
        include/components/trigger.h
        include/display/controls.h
        include/display/panel.h
        include/envelopes/states/stage.h
        include/envelopes/stage/deferring.h
        include/envelopes/stage/resting.h
        include/envelopes/stage/generating.h
        include/envelopes/hostage/sustaining.h
        include/util/duration.h
        include/util/mode.h
        include/util/range.h
        include/util/rotation.h
        include/util/selector.h
        include/util/sigmoid.h
        include/util/signal.h
        )
target_include_directories(components
        INTERFACE
        include
        )

add_library(modules OBJECT)
set_target_properties(modules
        PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        PREFIX ""
        )
target_sources(modules
        PRIVATE
        include/dhe-modules.h
        src/booster-stage.cpp
        src/cubic.cpp
        src/func.cpp
        src/hostage.cpp
        src/ranger.cpp
        src/stage.cpp
        src/swave.cpp
        src/tapers.cpp
        src/upstage.cpp
        src/xycloid.cpp
        )
target_compile_options(modules
        PUBLIC
        -stdlib=libc++
        -march=nocona
        -MMD
        -MP
        -O3
        -ffast-math
        -fno-finite-math-only
        -mmacosx-version-min=10.7
        -Wall
        -Wextra
        -Wno-unused-parameter
        )
target_compile_definitions(modules
        PUBLIC
        ARCH_MAC
        PRIVATE
        SLUG=DHE-Modules
        VERSION=0.6.5
        )
target_link_libraries(modules
        PUBLIC
        components
        racksdk
        )
target_link_options(modules
        PUBLIC
        "LINKER:-undefined,dynamic_lookup"
        )

add_library(plugin)
target_sources(plugin
        PRIVATE
        src/dhe-modules.cpp
        )
set_target_properties(plugin
        PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        PREFIX ""
        )
target_link_libraries(plugin
        PUBLIC
        modules
        racksdk
        )
target_link_options(plugin
        PUBLIC
        "LINKER:-undefined,dynamic_lookup"
        )




###############################################################################
#
# Tests
#
###############################################################################

add_executable(run-tests)
set_target_properties(run-tests
        PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        PREFIX ""
        )
target_link_libraries(run-tests
        PRIVATE
        components
        gmock_main
        )
target_include_directories(run-tests
        PRIVATE
        tests
        )
target_sources(run-tests
        PRIVATE
        tests/run-tests.cpp
        tests/defer-gate-tests.cpp
        )




###############################################################################
#
# Install to run in Rack
#
###############################################################################

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/rack)
set(PLUGINS_INSTALL_DIR plugins)
set(PLUGIN_INSTALL_DIR plugins/${PROJECT_NAME})

install(TARGETS plugin
        LIBRARY DESTINATION ${PLUGIN_INSTALL_DIR}
        ARCHIVE DESTINATION ${PLUGIN_INSTALL_DIR})
install(DIRECTORY ../svg DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES ../LICENSE.txt DESTINATION ${PLUGIN_INSTALL_DIR})
install(DIRECTORY /Users/Dale/Documents/Rack/plugins/Fundamental DESTINATION ${PLUGINS_INSTALL_DIR})




###############################################################################
#
# Rack SDK
#
###############################################################################

add_library(racksdk INTERFACE)
target_include_directories(racksdk
        INTERFACE
        ../../Rack-SDK/include
        ../../Rack-SDK/dep/include
        )
target_compile_features(racksdk
        INTERFACE
        cxx_std_11
        )



###############################################################################
#
# Google Test and Google Mock
#
###############################################################################

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download" )
execute_process(COMMAND "${CMAKE_COMMAND}" --build .
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download" )

# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This adds
# the following targets: gtest, gtest_main, gmock
# and gmock_main
add_subdirectory("${CMAKE_BINARY_DIR}/googletest-src"
        "${CMAKE_BINARY_DIR}/googletest-build")

# The gtest/gmock targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if(CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include"
            "${gmock_SOURCE_DIR}/include")
endif()

# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Now simply link your own targets against gtest, gmock,
# etc. as appropriate
