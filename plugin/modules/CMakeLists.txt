cmake_minimum_required(VERSION 3.15)

add_subdirectory(blossom)
add_subdirectory(cubic)
add_subdirectory(curve-sequencer)
add_subdirectory(func)
add_subdirectory(fuzzy-logic)
add_subdirectory(gator)
add_subdirectory(ranger)
add_subdirectory(stage)
add_subdirectory(swave)
add_subdirectory(tapers)
add_subdirectory(xycloid)

add_library(modules OBJECT)
target_link_libraries(modules
        PUBLIC
        blossom
        cubic
        curve_sequencer
        func
        fuzzy_logic
        gator
        ranger
        stage
        swave
        tapers
        xycloid
        )
