add_compile_options(-Wall -Wextra -Wno-unused-parameter -g -O3 -std=c++11 -fPIC)

if (WIN32)
    add_compile_definitions(ARCH_WIN _USE_MATH_DEFINES)
endif ()

if (APPLE)
    add_compile_definitions(ARCH_MAC)
    add_compile_options(-mmacosx-version-min=10.7 -stdlib=libc++)
else ()
    add_compile_options(-Wsuggest-override)
endif ()

if (UNIX AND NOT APPLE)
    add_compile_definitions(ARCH_LIN)
endif ()

add_subdirectory(common)
add_subdirectory(curve-sequencer)
