cmake_minimum_required(VERSION 3.15)
project(example)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
enable_language(CXX C ASM)


MESSAGE(STATUS "operation system is ${CMAKE_SYSTEM}")
string(FIND ${CMAKE_SYSTEM} "Linux" SYSTEM)
if(SYSTEM EQUAL -1)
  string(FIND ${CMAKE_SYSTEM} "linux" SYSTEM) 
endif()

if(SYSTEM EQUAL -1)
  message(STATUS "当前系统为 Windows")
  set(TOOLCHAINPATH "C:/arm-gnu-toolchain-15.2.rel1-mingw-w64-x86_64-arm-none-eabi/bin")
else()
  message(STATUS "当前系统为 Linux")
  set(TOOLCHAINPATH "/home/stone_li/arm-gnu-toolchain-15.2.rel1-x86_64-arm-none-eabi/bin")
endif()


# Windows
#set(TOOLCHAINPATH "C:/arm-gnu-toolchain-15.2.rel1-mingw-w64-x86_64-arm-none-eabi/bin")
# Linux
#set(TOOLCHAINPATH "/home/stone_li/arm-gnu-toolchain-15.2.rel1-x86_64-arm-none-eabi/bin")

####################
set(CMAKE_C_COMPILER ${TOOLCHAINPATH}/arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAINPATH}/arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER ${TOOLCHAINPATH}/arm-none-eabi-as)

################
set(CMAKE_OBJCOPY ${TOOLCHAINPATH}/arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP ${TOOLCHAINPATH}/arm-none-eabi-objdump)
set(CMAKE_SIZE ${TOOLCHAINPATH}/arm-none-eabi-size)
set(CMAKE_AR ${TOOLCHAINPATH}/arm-none-eabi-ar)
set(CMAKE_RANLIB ${TOOLCHAINPATH}/arm-none-eabi-ranlib)

set(FREERTOS_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../..)
set(FREERTOS_BOOT_PATH ${FREERTOS_ROOT_PATH}/boot)
set(FREERTOS_UTILS_PATH ${FREERTOS_ROOT_PATH}/utils)
set(FREERTOS_DRIVER_PATH ${FREERTOS_ROOT_PATH}/driver)
set(FREERTOS_KERNEL_PATH ${FREERTOS_ROOT_PATH}/FreeRTOS-Kernel)
set(LINKER_SCRIPT ${FREERTOS_ROOT_PATH}/link.ld)
#set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/link.ld)



# Add the freertos_config for FreeRTOS-Kernel
add_library(freertos_config INTERFACE)

target_include_directories(freertos_config
    INTERFACE
    "../configuration"
    ${FREERTOS_ROOT_PATH}/include
)

#target_include_directories(freertos_config
#    INTERFACE
#    "../template_configuration"
#)

if (DEFINED FREERTOS_SMP_EXAMPLE AND FREERTOS_SMP_EXAMPLE STREQUAL "1")
    message(STATUS "Build FreeRTOS SMP example")
    # Adding the following configurations to build SMP template port
    add_compile_options( -DconfigNUMBER_OF_CORES=2 -DconfigUSE_PASSIVE_IDLE_HOOK=0 )
endif()


#### ARM Cortex-R52
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -v -Og -g -std=gnu99 -mcpu=cortex-r52 -mfloat-abi=hard -mlibarch=armv8-r+crc -march=armv8-r+crc -mfpu=fpv5-d16")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-builtin-printf -fomit-frame-pointer -fno-strict-aliasing -fno-builtin-memcpy -Wall -Wextra -fno-short-enums -nostdlib -D__FREERTOS__")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static ")
#--no-warn-rwx-segments
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=cortex-r52 -mfloat-abi=hard -mfpu=fpv5-sp-d16")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mcpu=cortex-r52 -mfloat-abi=hard -mfpu=fpv5-sp-d16")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -mcpu=cortex-r52 -mfloat-abi=hard -mfpu=fpv5-sp-d16")




# Select the heap port.  values between 1-4 will pick a heap.
set(FREERTOS_HEAP "4" CACHE STRING "" FORCE)

# Select the native compile PORT
#set(FREERTOS_PORT "TEMPLATE" CACHE STRING "" FORCE)
set(FREERTOS_PORT "GCC_ARM_CR52" CACHE STRING "" FORCE)

# Adding the FreeRTOS-Kernel subdirectory
add_subdirectory(${FREERTOS_KERNEL_PATH} FreeRTOS-Kernel)
add_subdirectory(${FREERTOS_ROOT_PATH} root)
#add_subdirectory(${FREERTOS_ROOT_PATH} driver)

########################################################################
# Overall Compile Options
# Note the compile option strategy is to error on everything and then
# Per library opt-out of things that are warnings/errors.
# This ensures that no matter what strategy for compilation you take, the
# builds will still occur.
#
# Only tested with GNU and Clang.
# Other options are https://cmake.org/cmake/help/latest/variable/CMAKE_LANG_COMPILER_ID.html#variable:CMAKE_%3CLANG%3E_COMPILER_ID
# Naming of compilers translation map:
#
#   FreeRTOS    | CMake
#   -------------------
#   CCS         | ?TBD?
#   GCC         | GNU, Clang, *Clang Others?
#   IAR         | IAR
#   Keil        | ARMCC
#   MSVC        | MSVC # Note only for MinGW?
#   Renesas     | ?TBD?

target_compile_options(freertos_kernel PRIVATE
    ### Gnu/Clang C Options
    $<$<COMPILE_LANG_AND_ID:C,GNU>:-fdiagnostics-color=always>
    $<$<COMPILE_LANG_AND_ID:C,Clang>:-fcolor-diagnostics>

    $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Wall>
    $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Wextra>
    $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Wpedantic>
    $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Werror>
    $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Wconversion>
    $<$<COMPILE_LANG_AND_ID:C,Clang>:-Weverything>

    # Suppressions required to build clean with clang.
    $<$<COMPILE_LANG_AND_ID:C,Clang>:-Wno-unused-macros>
    $<$<COMPILE_LANG_AND_ID:C,Clang>:-Wno-padded>
    $<$<COMPILE_LANG_AND_ID:C,Clang>:-Wno-missing-variable-declarations>
    $<$<COMPILE_LANG_AND_ID:C,Clang>:-Wno-covered-switch-default>
    $<$<COMPILE_LANG_AND_ID:C,Clang>:-Wno-cast-align> )



add_executable(${PROJECT_NAME}.elf
    ${FREERTOS_BOOT_PATH}/start.s
    main.c
)


# �������ӽű���Ϊ link.ld
target_link_options(${PROJECT_NAME}.elf PRIVATE
    -mcpu=cortex-r52
#    -mthumb
    -T ${LINKER_SCRIPT}
    -nostartfiles      # 
    -nostdlib          # 
    --specs=nosys.specs
#    -Wl,-eReset_Handler
    -Wl,--start-group
    -lgcc
    -lc
    -lm
    -Wl,--end-group
    -Wl,--gc-sections
)


target_link_libraries(${PROJECT_NAME}.elf 
    PUBLIC
        freertos_kernel_include
        freertos_kernel_port_headers
    PRIVATE
        freertos_kernel
        freertos_vcodec
        freertos_utils
#        freertos_boot
        freertos_driver
        freertos_config
)



# 7. ���������/hex�ļ�
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND arm-none-eabi-objcopy -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
    COMMAND arm-none-eabi-objcopy -O ihex ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex
    COMMENT "Generate bin & hex file"
)
