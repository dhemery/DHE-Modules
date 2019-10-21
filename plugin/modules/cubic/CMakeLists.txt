add_library(cubic)
target_include_directories(cubic PUBLIC .)
target_sources(cubic
        PRIVATE
        Cubic.cpp
        CubicPanel.cpp
        )
target_link_libraries(cubic
        PUBLIC
        components
        controls
        config
        widgets
        racksdk
        )
