cmake_minimum_required(VERSION 3.15)



########################################################################
add_subdirectory(include)
add_subdirectory(vcodec)
add_subdirectory(driver)
add_subdirectory(utils)



#target_link_libraries(freertos_vcodec
#    PUBLIC
#        freertos_kernel_include
#        freertos_kernel_port_headers
#    PRIVATE
#        freertos_kernel_port
#
#)

########################################################################
