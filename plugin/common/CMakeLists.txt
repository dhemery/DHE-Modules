add_library(components INTERFACE)
target_include_directories(components INTERFACE include)

add_library(controls STATIC)
target_include_directories(controls PUBLIC include)
target_sources(controls
        PRIVATE
        src/controls/CommonInputs.cpp
        src/controls/DurationInputs.cpp
        )
target_link_libraries(controls
        PUBLIC
        components
        )

add_library(config STATIC)
target_include_directories(config PUBLIC include)
target_sources(config
        PRIVATE
        src/config/CurvatureConfig.cpp
        src/config/DurationConfig.cpp
        src/config/LevelConfig.cpp
        )
target_link_libraries(config
        PUBLIC
        components
        controls
        Rack::SDK
        )

add_library(widgets INTERFACE)
target_include_directories(widgets INTERFACE include)
