# FreeRTOS internal cmake file. Do not use it in user top-level project

add_library(freertos_driver_include INTERFACE)

target_include_directories(freertos_driver_include  INTERFACE  ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(freertos_driver_include
    INTERFACE
)
