cmake_minimum_required(VERSION 3.15)

add_library(config OBJECT)
target_include_directories(config PUBLIC .)
target_sources(config
        PRIVATE
        CurvatureConfig.cpp
        DurationConfig.cpp
        LevelConfig.cpp
        )
target_link_libraries(config
        PUBLIC
        components
        controls
        racksdk
        )
set_target_properties(config
        PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS YES
        )
