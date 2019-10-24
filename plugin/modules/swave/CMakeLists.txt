add_library(swave
        Swave.cpp
        SwavePanel.cpp
        )
target_include_directories(swave PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(swave
        PUBLIC
        components
        controls
        config
        widgets
        racksdk
        )
