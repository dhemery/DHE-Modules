add_library(config)
target_include_directories(config INTERFACE .)
target_compile_features(config PUBLIC cxx_std_11)
target_sources(config
        PRIVATE
        CurvatureConfig.cpp
        DurationConfig.cpp
        LevelConfig.cpp
        )
target_link_libraries(config
        PUBLIC
        components
        controls
        racksdk
        )
