cmake_minimum_required(VERSION 3.15)
project(DHE-Modules
        VERSION 1.6.0
        LANGUAGES CXX)
enable_testing()

add_compile_options(
        -Wall -Wextra -Wno-unused-parameter
        -g

        # Lin and Win only
        # -Wsuggest-override

        # Mac only (FLAGS and LDFLAGS)
        -mmacosx-version-min=10.7

        -stdlib=libc++
)

add_compile_definitions(
        # ARCH_LIN # Linux Only
        # ARCH_WIN # Windows Only
        ARCH_MAC # Mac Only

        # Win only
        # _USE_MATH_DEFINES
)

set_property(GLOBAL PROPERTY
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS YES
        )

include(FetchContent)

FetchContent_Declare(racksdk
        URL https://vcvrack.com/downloads/Rack-SDK-1.1.5.zip
        )

FetchContent_GetProperties(racksdk)
if(NOT racksdk_POPULATED)
    FetchContent_Populate(racksdk)
endif()

add_library(racksdk INTERFACE)
target_include_directories(racksdk
        INTERFACE
        ${racksdk_SOURCE_DIR}/include
        ${racksdk_SOURCE_DIR}/dep/include
        )
target_compile_features(racksdk INTERFACE cxx_std_11)

include(GoogleTest)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.10.0
)

FetchContent_MakeAvailable(googletest)

add_subdirectory(components)
add_subdirectory(controls)
add_subdirectory(config)
add_subdirectory(widgets)

add_subdirectory(modules/blossom)
add_subdirectory(modules/cubic)
add_subdirectory(modules/curve-sequencer)
add_subdirectory(modules/func)
add_subdirectory(modules/fuzzy-logic)
add_subdirectory(modules/gator)
add_subdirectory(modules/ranger)
add_subdirectory(modules/stage)
add_subdirectory(modules/swave)
add_subdirectory(modules/tapers)
add_subdirectory(modules/xycloid)

add_library(plugin SHARED)
target_sources(plugin
        PRIVATE
        PluginInitializer.cpp
        )

target_link_libraries(plugin
        PRIVATE
        blossom
        cubic
        curve_sequencer
        func
        fuzzy_logic
        gator
        ranger
        stage
        swave
        tapers
        xycloid
        )
target_link_options(plugin
        PRIVATE

        # Mac only
        "LINKER:-undefined,dynamic_lookup"

        # Win only
        # -L$(RACK_DIR) -lRack
        )
set_target_properties(plugin
        PROPERTIES
        PREFIX ""
        )

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/rack)
set(PLUGINS_INSTALL_DIR plugins-v1)
set(PLUGIN_INSTALL_DIR plugins-v1/${PROJECT_NAME})

install(TARGETS plugin
        LIBRARY DESTINATION ${PLUGIN_INSTALL_DIR}
        ARCHIVE DESTINATION ${PLUGIN_INSTALL_DIR}
        )
install(DIRECTORY ../svg DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES ../LICENSE.txt ../plugin.json DESTINATION ${PLUGIN_INSTALL_DIR})
