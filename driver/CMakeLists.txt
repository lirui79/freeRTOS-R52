cmake_minimum_required(VERSION 3.15)


add_library(freertos_driver STATIC)

########################################################################
## add_subdirectory(include)
## add_subdirectory(portable)

target_sources(freertos_driver PRIVATE
    gic.c
    mhu_v3_x.c
    mhu_wrapper_v3_x.c
    uart.c
)

target_link_libraries(freertos_driver
    PUBLIC
        freertos_config
        freertos_kernel_include
        freertos_kernel_port_headers
        freertos_driver_include

)

########################################################################
