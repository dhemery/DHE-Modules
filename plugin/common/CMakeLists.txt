add_library(components INTERFACE)
add_library(dhe::components ALIAS components)
target_include_directories(components INTERFACE include)

add_library(controls STATIC)
add_library(dhe::controls ALIAS controls)
target_include_directories(controls PUBLIC include)
target_sources(controls
        PRIVATE
        src/controls/CommonInputs.cpp
        src/controls/DurationInputs.cpp
        )
target_link_libraries(controls
        PUBLIC
        dhe::components
        )

add_library(config STATIC)
add_library(dhe::config ALIAS config)
target_include_directories(config PUBLIC include)
target_sources(config
        PRIVATE
        src/config/CurvatureConfig.cpp
        src/config/DurationConfig.cpp
        src/config/LevelConfig.cpp
        )
target_link_libraries(config
        PUBLIC
        dhe::components
        dhe::controls
        Rack::SDK
        )

add_library(widgets INTERFACE)
add_library(dhe::widgets ALIAS widgets)
target_include_directories(widgets INTERFACE include)
