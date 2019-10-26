add_library(xycloid
        Xycloid.cpp
        XycloidPanel.cpp
        )
target_include_directories(cubic PUBLIC .)
target_link_libraries(xycloid
        PUBLIC
        components
        config
        controls
        widgets
        Rack::SDK
        )
