set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_compile_options(
        -Wall -Wextra -Wno-unused-parameter -g -O3
        $<$<STREQUAL:${CMAKE_HOST_SYSTEM_NAME},Darwin>:-mmacosx-version-min=10.7>
        $<$<STREQUAL:${CMAKE_HOST_SYSTEM_NAME},Darwin>:-stdlib=libc++>
        $<$<STREQUAL:${CMAKE_HOST_SYSTEM_NAME},Linux>:-Wsuggest-override>
        $<$<STREQUAL:${CMAKE_HOST_SYSTEM_NAME},Windows>:-Wsuggest-override>
)

add_compile_definitions(
        $<$<STREQUAL:${CMAKE_HOST_SYSTEM_NAME},Darwin>:ARCH_MAC>
        $<$<STREQUAL:${CMAKE_HOST_SYSTEM_NAME},Linux>:ARCH_LIN>
        $<$<STREQUAL:${CMAKE_HOST_SYSTEM_NAME},Windows>:ARCH_WIN>
        $<$<STREQUAL:${CMAKE_HOST_SYSTEM_NAME},Windows>:_USE_MATH_DEFINES>
)

add_subdirectory(components)
add_subdirectory(config)
add_subdirectory(controls)
add_subdirectory(widgets)
add_subdirectory(modules)
add_subdirectory(test)

add_library(plugin SHARED)
target_include_directories(plugin
        PUBLIC
        .
        )
target_sources(plugin
        PRIVATE
        PluginInitializer.cpp
        )
target_link_libraries(plugin
        PRIVATE
        dhe::components
        dhe::controls
        dhe::config
        dhe::blossom
        dhe::cubic
        dhe::curve_sequencer
        dhe::func
        dhe::fuzzy_logic
        dhe::gator
        dhe::ranger
        dhe::stage
        dhe::swave
        dhe::tapers
        dhe::xycloid
        Rack::SDK
        )
IF (APPLE)
    target_link_options(plugin PUBLIC "LINKER:-undefined,dynamic_lookup")
endif ()

set_target_properties(plugin PROPERTIES PREFIX "")

install(TARGETS plugin
        LIBRARY DESTINATION ${RACK_PLUGINS_DIR}
        ARCHIVE DESTINATION ${RACK_PLUGINS_DIR}
        )
