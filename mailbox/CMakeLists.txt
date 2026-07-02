cmake_minimum_required(VERSION 3.15)

add_library(freertos_mailbox_include INTERFACE)

target_include_directories(freertos_mailbox_include  INTERFACE  ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(freertos_mailbox_include
    INTERFACE
)

add_library(freertos_mailbox STATIC)

########################################################################
## add_subdirectory(include)
## add_subdirectory(portable)

target_sources(freertos_mailbox PRIVATE
    cmdr52_proc.c
    cmdr52_mgr.c
    cmdr52_session.c
)

target_link_libraries(freertos_mailbox
    PUBLIC
        freertos_config
        freertos_kernel_include
        freertos_kernel_port_headers
        freertos_vcodec_include
        freertos_driver_include
        freertos_mailbox_include
    PRIVATE
        freertos_utils
        freertos_vcodec
        freertos_driver
)

########################################################################
