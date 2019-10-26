add_library(blossom
        Blossom.cpp
        BlossomPanel.cpp
        )
target_include_directories(blossom PUBLIC .)
target_link_libraries(blossom
        PUBLIC
        components
        controls
        config
        widgets
        Rack::SDK
        )
