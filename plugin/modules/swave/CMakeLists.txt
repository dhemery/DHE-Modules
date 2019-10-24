add_library(swave
        Swave.cpp
        SwavePanel.cpp
        )
target_include_directories(swave PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(swave
        PUBLIC
        components
        controls
        config
        widgets
        racksdk
        )
