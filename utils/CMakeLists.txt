cmake_minimum_required(VERSION 3.15)


add_library(freertos_utils STATIC)

########################################################################
## add_subdirectory(include)
## add_subdirectory(portable)

target_sources(freertos_utils PRIVATE
    libc.c
    crc32.c
    printf.c
    cqueue.c
    bqueue.c
)

target_link_libraries(freertos_utils
    PUBLIC
        freertos_config
        freertos_kernel_include
        freertos_kernel_port_headers
        freertos_driver_include
        freertos_vcodec_include

)

########################################################################
