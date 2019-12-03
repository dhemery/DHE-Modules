add_library(swave INTERFACE)
add_library(dhe::swave ALIAS swave)
target_link_libraries(swave
        INTERFACE
        dhe::components
        dhe::controls
        dhe::config
        dhe::widgets
        Rack::SDK
        )