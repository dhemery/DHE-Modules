add_library(ranger STATIC)
add_library(dhe::ranger ALIAS ranger)
target_sources(ranger
        PRIVATE
        Ranger.cpp
        RangerPanel.cpp
        )
target_link_libraries(ranger
        PUBLIC
        dhe::components
        dhe::controls
        dhe::config
        dhe::widgets
        Rack::SDK
        )