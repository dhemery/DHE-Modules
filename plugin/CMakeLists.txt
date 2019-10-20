cmake_minimum_required(VERSION 3.15)
project(DHE-Modules
        VERSION 1.6.0
        LANGUAGES CXX)

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

include(GoogleTest)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.10.0
)

FetchContent_MakeAvailable(googletest)
enable_testing()

add_subdirectory(components)
add_subdirectory(controls)
add_subdirectory(config)
add_subdirectory(modules)
add_subdirectory(widgets)

add_library(plugin SHARED)
target_sources(plugin
        PRIVATE
        PluginInitializer.cpp
        )

target_link_libraries(plugin
        PRIVATE
        modules
        )
set_target_properties(plugin
        PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        PREFIX ""
        )
target_link_options(plugin
        PRIVATE

        # Mac only
        "LINKER:-undefined,dynamic_lookup"

        # Win only
        # -L$(RACK_DIR) -lRack
        )
