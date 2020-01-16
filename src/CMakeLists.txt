include(${CMAKE_SOURCE_DIR}/cmake/compile-options.cmake)

add_subdirectory(components)
add_subdirectory(config)
add_subdirectory(controls)
add_subdirectory(widgets)
add_subdirectory(modules)

add_library(plugin SHARED)
target_include_directories(plugin
        PUBLIC
        .
        )
target_sources(plugin
        PRIVATE
        Plugin.cpp
        )
target_link_libraries(plugin
        PRIVATE
        dhe::blossom
        dhe::components
        dhe::config
        dhe::controls
        dhe::cubic
        dhe::curve_sequencer
        dhe::func
        dhe::fuzzy_logic
        dhe::gator
        dhe::ranger
        dhe::stage
        dhe::swave
        dhe::tapers
        dhe::widgets
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
