add_library(swave)
target_include_directories(swave PUBLIC .)
target_sources(swave
        PRIVATE
        Swave.cpp
        SwavePanel.cpp
        )
target_link_libraries(swave
        PUBLIC
        components
        controls
        config
        widgets
        racksdk
        )
