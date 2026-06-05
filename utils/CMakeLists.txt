cmake_minimum_required(VERSION 3.15)


add_library(freertos_utils STATIC)

########################################################################
## add_subdirectory(include)
## add_subdirectory(portable)

target_sources(freertos_utils PRIVATE
    libc.c
    crc32.c
    printf.c
)

target_link_libraries(freertos_utils
    PUBLIC
        freertos_driver_include

)

########################################################################
