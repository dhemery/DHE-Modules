add_library(swave STATIC)
target_sources(swave
        PRIVATE
        Swave.cpp
        SwavePanel.cpp
        )
target_link_libraries(swave
        PUBLIC
        components
        controls
        config
        widgets
        Rack::SDK
        )