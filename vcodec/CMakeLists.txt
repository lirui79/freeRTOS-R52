cmake_minimum_required(VERSION 3.15)


add_library(freertos_vcodec STATIC)

########################################################################
## add_subdirectory(include)
## add_subdirectory(portable)
target_include_directories(freertos_vcodec 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

add_compile_options( -D__FREERTOS__ )

target_sources(freertos_vcodec PRIVATE
    bidirect_list.c
    vcd_vcmd_pm.c
    vce_vcmd_pm.c
    vcx_axi2to1.c
    vcx_irq_simulation.c
    vcx_vcmd_dbg_log.c
    vcx_watchdog.c
    vcd_abnormal_irq.c
    vce_abnormal_irq.c
    vcmdswhwregisters.c
    vcx_axife.c
    vcx_irq_timer.c
    vcx_vcmd_dbgfs.c
    vcd_vcmd.c
    vce_vcmd.c
    vcodec.c
    vcx_cmdbuf_obj.c
    vcx_kthread.c
    vcx_vcmd_irq.c
)

target_link_libraries(freertos_vcodec
    PUBLIC
        freertos_config
        freertos_kernel_include
        freertos_kernel_port_headers
        freertos_driver_include

)

########################################################################
