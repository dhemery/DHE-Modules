add_library(xycloid)
target_include_directories(xycloid PUBLIC .)
target_sources(xycloid
        PRIVATE
        Xycloid.cpp
        XycloidPanel.cpp
        )
target_link_libraries(xycloid
        PUBLIC
        components
        config
        controls
        widgets
        racksdk
        )
