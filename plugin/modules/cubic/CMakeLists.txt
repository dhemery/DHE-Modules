add_library(cubic STATIC)
target_sources(cubic
        PRIVATE
        Cubic.cpp
        CubicPanel.cpp
        )
target_link_libraries(cubic
        PUBLIC
        components
        controls
        config
        widgets
        Rack::SDK
        )