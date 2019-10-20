cmake_minimum_required(VERSION 3.15)

add_library(swave OBJECT)
target_include_directories(swave PUBLIC .)
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
        racksdk
        )
set_target_properties(swave
        PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS YES
        )
