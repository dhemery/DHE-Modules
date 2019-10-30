add_library(cubic STATIC)
add_library(dhe::cubic ALIAS cubic)
target_sources(cubic
        PRIVATE
        Cubic.cpp
        CubicPanel.cpp
        )
target_link_libraries(cubic
        PUBLIC
        dhe::components
        dhe::controls
        dhe::config
        dhe::widgets
        Rack::SDK
        )