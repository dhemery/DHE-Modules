add_library(xycloid STATIC)
target_sources(xycloid
        PRIVATE
        Xycloid.cpp
        XycloidPanel.cpp
        )
target_link_libraries(xycloid
        PUBLIC
        components
        controls
        config
        widgets
        Rack::SDK
        )