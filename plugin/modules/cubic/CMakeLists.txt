cmake_minimum_required(VERSION 3.15)

add_library(cubic OBJECT)
target_include_directories(cubic PUBLIC .)
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
        racksdk
        )
set_target_properties(cubic
        PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS YES
        )
