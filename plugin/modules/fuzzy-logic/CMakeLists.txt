cmake_minimum_required(VERSION 3.15)

add_library(fuzzy_logic)
target_include_directories(fuzzy_logic PUBLIC .)
target_sources(fuzzy_logic
        PRIVATE
        FuzzyLogicH.cpp
        FuzzyLogicHPanel.cpp
        FuzzyLogicZ.cpp
        FuzzyLogicZPanel.cpp
        )
target_link_libraries(fuzzy_logic
        PUBLIC
        components
        controls
        config
        widgets
        racksdk
        )
