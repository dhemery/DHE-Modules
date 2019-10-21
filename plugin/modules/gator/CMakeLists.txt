cmake_minimum_required(VERSION 3.15)

add_library(gator)
target_include_directories(gator PUBLIC .)
target_sources(gator
        PRIVATE
        Gator.cpp
        GatorPanel.cpp
        )
target_link_libraries(gator
        PUBLIC
        components
        controls
        config
        widgets
        racksdk
        )
