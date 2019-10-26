add_library(components INTERFACE)
target_compile_features(components INTERFACE cxx_std_11)
target_include_directories(components INTERFACE .)
