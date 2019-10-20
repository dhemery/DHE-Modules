cmake_minimum_required(VERSION 3.15)

add_library(ranger OBJECT)
target_include_directories(ranger PUBLIC .)
target_sources(ranger
        PRIVATE
        Ranger.cpp
        RangerPanel.cpp
        )
target_link_libraries(ranger
        PUBLIC
        components
        controls
        config
        widgets
        racksdk
        )
set_target_properties(ranger
        PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS YES
        )
