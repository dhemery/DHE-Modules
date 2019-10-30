add_library(gator STATIC)
add_library(dhe::gator ALIAS gator)
target_sources(gator
        PRIVATE
        Gator.cpp
        GatorPanel.cpp
        )
target_link_libraries(gator
        PUBLIC
        dhe::components
        dhe::controls
        dhe::config
        dhe::widgets
        Rack::SDK
        )