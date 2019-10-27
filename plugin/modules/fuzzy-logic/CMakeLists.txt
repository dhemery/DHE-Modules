add_library(fuzzy_logic STATIC)
target_sources(fuzzy_logic
        PRIVATE
        FuzzyLogicH.cpp
        FuzzyLogicZ.cpp
        FuzzyLogicHPanel.cpp
        FuzzyLogicZPanel.cpp
        )
target_link_libraries(fuzzy_logic
        PUBLIC
        components
        controls
        config
        widgets
        Rack::SDK
        )