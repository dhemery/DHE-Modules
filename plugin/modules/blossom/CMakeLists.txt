add_library(blossom)
target_include_directories(blossom PUBLIC .)
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
        )
