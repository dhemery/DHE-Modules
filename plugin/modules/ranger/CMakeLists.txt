add_library(ranger STATIC)
target_sources(ranger
        PRIVATE
        Ranger.cpp
        RangerPanel.cpp
        )
target_link_libraries(ranger
        PUBLIC
        components
        controls
        config
        widgets
        Rack::SDK
        )