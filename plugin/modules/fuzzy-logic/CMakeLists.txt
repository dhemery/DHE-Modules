cmake_minimum_required(VERSION 3.15)

add_library(fuzzy_logic OBJECT)
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
set_target_properties(fuzzy_logic
        PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS YES
        )
