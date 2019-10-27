set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_compile_options(-Wall -Wextra -Wno-unused-parameter -g -O3)

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
add_subdirectory(modules)
add_subdirectory(test)
