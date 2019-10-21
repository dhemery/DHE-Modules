add_library(widgets INTERFACE)
target_include_directories(widgets INTERFACE .)
target_link_libraries(widgets INTERFACE racksdk)
