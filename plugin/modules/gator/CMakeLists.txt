add_library(gator STATIC)
target_sources(gator
        PRIVATE
        Gator.cpp
        GatorPanel.cpp
        )
target_link_libraries(gator
        PUBLIC
        components
        controls
        config
        widgets
        Rack::SDK
        )