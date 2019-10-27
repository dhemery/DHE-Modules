add_library(blossom STATIC)
target_sources(blossom
        PRIVATE
        Blossom.cpp
        BlossomPanel.cpp
        )
target_link_libraries(blossom
        PUBLIC
        components
        controls
        config
        widgets
        Rack::SDK
        )