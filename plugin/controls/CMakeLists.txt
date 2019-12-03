add_library(controls INTERFACE)
add_library(dhe::controls ALIAS controls)
target_link_libraries(controls
        INTERFACE
        dhe::components
        )
