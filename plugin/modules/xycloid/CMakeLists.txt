add_library(xycloid
        Xycloid.cpp
        XycloidPanel.cpp
        )
target_include_directories(xycloid PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(xycloid
        PUBLIC
        components
        config
        controls
        widgets
        racksdk
        )
