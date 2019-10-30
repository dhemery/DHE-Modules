add_library(fuzzy_logic STATIC)
add_library(dhe::fuzzy_logic ALIAS fuzzy_logic)
target_sources(fuzzy_logic
        PRIVATE
        FuzzyLogicH.cpp
        FuzzyLogicZ.cpp
        FuzzyLogicHPanel.cpp
        FuzzyLogicZPanel.cpp
        )
target_link_libraries(fuzzy_logic
        PUBLIC
        dhe::components
        dhe::controls
        dhe::config
        dhe::widgets
        Rack::SDK
        )