cmake_minimum_required(VERSION 3.15)

add_library(xycloid)
target_include_directories(xycloid PUBLIC .)
target_sources(xycloid
        PRIVATE
        Xycloid.cpp
        XycloidPanel.cpp
        )
target_link_libraries(xycloid
        PUBLIC
        components
        controls
        config
        widgets
        racksdk
        )
set_target_properties(xycloid
        PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        )
