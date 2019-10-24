add_library(cubic
        Cubic.cpp
        CubicPanel.cpp
        )
target_include_directories(cubic PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(cubic
        PUBLIC
        components
        controls
        config
        widgets
        racksdk
        )
