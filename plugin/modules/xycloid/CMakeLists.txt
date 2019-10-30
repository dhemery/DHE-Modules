add_library(xycloid STATIC)
add_library(dhe::xycloid ALIAS xycloid)
target_sources(xycloid
        PRIVATE
        Xycloid.cpp
        XycloidPanel.cpp
        )
target_link_libraries(xycloid
        PUBLIC
        dhe::components
        dhe::controls
        dhe::config
        dhe::widgets
        Rack::SDK
        )