set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_compile_options(
        -Wall -Wextra -Wno-unused-parameter -g -O3
        $<$<STREQUAL:${CMAKE_HOST_SYSTEM_NAME},Darwin>:-mmacosx-version-min=10.7>
        $<$<STREQUAL:${CMAKE_HOST_SYSTEM_NAME},Darwin>:-stdlib=libc++>
        $<$<STREQUAL:${CMAKE_HOST_SYSTEM_NAME},Linux>:-Wsuggest-override>
        $<$<STREQUAL:${CMAKE_HOST_SYSTEM_NAME},Windows>:-Wsuggest-override>
)

add_compile_definitions(
        $<$<STREQUAL:${CMAKE_HOST_SYSTEM_NAME},Darwin>:ARCH_MAC>
        $<$<STREQUAL:${CMAKE_HOST_SYSTEM_NAME},Linux>:ARCH_LIN>
        $<$<STREQUAL:${CMAKE_HOST_SYSTEM_NAME},Windows>:ARCH_WIN>
        $<$<STREQUAL:${CMAKE_HOST_SYSTEM_NAME},Windows>:_USE_MATH_DEFINES>
)
