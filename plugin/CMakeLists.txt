cmake_minimum_required(VERSION 3.12.4)

project(DHE-Modules VERSION 1.6.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

add_compile_options(-Wall -Wextra -Wno-unused-parameter -g -O3 -std=c++11 -fPIC)

if (APPLE)
    add_compile_definitions(ARCH_MAC)
    add_compile_options(-mmacosx-version-min=10.7 -stdlib=libc++)
endif ()

if (UNIX AND NOT APPLE)
    add_compile_definitions(ARCH_LIN)
    add_compile_options(-Wsuggest-override)
endif ()

if (NOT UNIX)
    add_compile_definitions(ARCH_WIN _USE_MATH_DEFINES)
    add_compile_options(-Wsuggest-override)
endif ()

include(FetchContent)

FetchContent_Declare(racksdk URL https://vcvrack.com/downloads/Rack-SDK-1.1.5.zip)

FetchContent_GetProperties(racksdk)
if (NOT racksdk_POPULATED)
    FetchContent_Populate(racksdk)
endif ()

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

FetchContent_GetProperties(googletest)
if (NOT googletest_POPULATED)
    FetchContent_Populate(googletest)
    add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif ()

enable_testing()

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
IF (NOT UNIX)
    target_link_options(plugin PRIVATE -L${racksdk_SOURCE_DIR} -lRack)
endif ()
IF (APPLE)
    target_link_options(plugin PRIVATE "LINKER:-undefined,dynamic_lookup")
endif ()
set_target_properties(plugin PROPERTIES PREFIX "")

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/rack)
set(PLUGINS_INSTALL_DIR plugins-v1)
set(PLUGIN_INSTALL_DIR plugins-v1/${PROJECT_NAME})

install(TARGETS plugin
        LIBRARY DESTINATION ${PLUGIN_INSTALL_DIR}
        ARCHIVE DESTINATION ${PLUGIN_INSTALL_DIR}
        )
install(DIRECTORY ../svg DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES ../LICENSE.txt ../plugin.json DESTINATION ${PLUGIN_INSTALL_DIR})
