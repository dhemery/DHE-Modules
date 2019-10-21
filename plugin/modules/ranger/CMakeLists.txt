add_library(ranger)
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
