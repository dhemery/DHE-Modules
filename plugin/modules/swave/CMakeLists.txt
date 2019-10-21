add_library(swave
        Swave.cpp
        SwavePanel.cpp
        )
target_include_directories(swave PUBLIC .)
target_link_libraries(swave
        PUBLIC
        components
        controls
        config
        widgets
        racksdk
        )
