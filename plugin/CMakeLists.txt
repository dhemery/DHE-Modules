cmake_minimum_required(VERSION 3.15)

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
        CXX_EXTENSIONS YES
        PREFIX ""
        )
target_link_options(plugin
        PRIVATE

        # Mac only
        "LINKER:-undefined,dynamic_lookup"

        # Win only
        # -L$(RACK_DIR) -lRack
        )
