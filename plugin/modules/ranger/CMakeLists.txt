add_library(ranger
        Ranger.cpp
        RangerPanel.cpp
        )
target_include_directories(cubic PUBLIC .)
target_link_libraries(ranger
        PUBLIC
        components
        controls
        config
        widgets
        Rack::SDK
        )
