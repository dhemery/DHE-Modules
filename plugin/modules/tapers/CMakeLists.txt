cmake_minimum_required(VERSION 3.15)

add_library(tapers OBJECT)
target_include_directories(tapers PUBLIC .)
target_sources(tapers
        PRIVATE
        Tapers.cpp
        TapersPanel.cpp
        )
target_link_libraries(tapers
        PUBLIC
        components
        controls
        config
        widgets
        racksdk
        )
set_target_properties(tapers
        PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS YES
        )
