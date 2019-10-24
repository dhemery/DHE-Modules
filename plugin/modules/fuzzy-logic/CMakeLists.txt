add_library(fuzzy_logic
        FuzzyLogicH.cpp
        FuzzyLogicHPanel.cpp
        FuzzyLogicZ.cpp
        FuzzyLogicZPanel.cpp
        )
target_include_directories(fuzzy_logic PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(fuzzy_logic
        PUBLIC
        components
        controls
        config
        widgets
        racksdk
        )
