add_library(tapers STATIC)
add_library(dhe::tapers ALIAS tapers)
target_sources(tapers
        PRIVATE
        Tapers.cpp
        TapersPanel.cpp
        )
target_link_libraries(tapers
        PUBLIC
        dhe::components
        dhe::controls
        dhe::config
        dhe::widgets
        Rack::SDK
        )