add_library(fuzzy_logic INTERFACE)
add_library(dhe::fuzzy_logic ALIAS fuzzy_logic)
target_link_libraries(fuzzy_logic
        INTERFACE
        dhe::components
        dhe::controls
        dhe::config
        dhe::widgets
        Rack::SDK
        )