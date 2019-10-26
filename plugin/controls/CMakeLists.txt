add_library(controls
        CommonInputs.cpp
        DurationInputs.cpp
        )
target_include_directories(controls PUBLIC .)
target_link_libraries(controls
        PUBLIC
        components
        Rack::SDK
        )
