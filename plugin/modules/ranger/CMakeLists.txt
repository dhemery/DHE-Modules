add_library(ranger
        Ranger.cpp
        RangerPanel.cpp
        )
target_include_directories(ranger PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(ranger
        PUBLIC
        components
        controls
        config
        widgets
        racksdk
        )
