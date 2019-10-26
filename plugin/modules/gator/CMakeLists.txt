add_library(gator
        Gator.cpp
        GatorPanel.cpp
        )
target_include_directories(cubic PUBLIC .)
target_link_libraries(gator
        PUBLIC
        components
        controls
        config
        widgets
        Rack::SDK
        )
