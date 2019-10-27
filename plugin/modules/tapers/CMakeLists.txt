add_library(tapers STATIC)
target_sources(tapers
        PRIVATE
        Tapers.cpp
        TapersPanel.cpp
        )
target_link_libraries(tapers
        PUBLIC
        components
        controls
        config
        widgets
        Rack::SDK
        )