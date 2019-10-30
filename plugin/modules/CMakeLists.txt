add_subdirectory(blossom)
add_subdirectory(cubic)
add_subdirectory(curve-sequencer)
add_subdirectory(func)
add_subdirectory(fuzzy-logic)
add_subdirectory(gator)
add_subdirectory(ranger)
add_subdirectory(stage)
add_subdirectory(swave)
add_subdirectory(tapers)
add_subdirectory(xycloid)

add_library(plugin SHARED)
target_include_directories(plugin PUBLIC .)
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
