add_library(tapers
        Tapers.cpp
        TapersPanel.cpp
        )
target_include_directories(tapers PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(tapers
        PUBLIC
        components
        controls
        config
        widgets
        racksdk
        )
