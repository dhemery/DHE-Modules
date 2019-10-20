cmake_minimum_required(VERSION 3.15)

add_library(blossom OBJECT)
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
set_target_properties(blossom
        PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS YES
        )
