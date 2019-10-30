add_library(swave STATIC)
add_library(dhe::swave ALIAS swave)
target_sources(swave
        PRIVATE
        Swave.cpp
        SwavePanel.cpp
        )
target_link_libraries(swave
        PUBLIC
        dhe::components
        dhe::controls
        dhe::config
        dhe::widgets
        Rack::SDK
        )