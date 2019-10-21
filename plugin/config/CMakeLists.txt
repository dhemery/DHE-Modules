add_library(config
        CurvatureConfig.cpp
        DurationConfig.cpp
        LevelConfig.cpp
        )
target_include_directories(config INTERFACE .)
target_compile_features(config PUBLIC cxx_std_11)
target_link_libraries(config
        PUBLIC
        components
        controls
        racksdk
        )
