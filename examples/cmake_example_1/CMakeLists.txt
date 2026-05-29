cmake_minimum_required(VERSION 3.15)
project(example)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Windows
set(TOOLCHAINPATH "C:/arm-gnu-toolchain-15.2.rel1-mingw-w64-x86_64-arm-none-eabi/bin")
# Linux
#set(TOOLCHAINPATH "/home/stone_li/arm-gnu-toolchain-15.2.rel1-x86_64-arm-none-eabi/bin")

# 你的 ARM 编译器路径（和你命令里一致）
set(CMAKE_C_COMPILER ${TOOLCHAINPATH}/arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAINPATH}/arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER ${TOOLCHAINPATH}/arm-none-eabi-as)

# 编译器工具
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
#set(LINKER_SCRIPT ${FREERTOS_ROOT_PATH}/link.ld)
set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/link.ld)

if(NOT EXISTS ${LINKER_SCRIPT})
    message(FATAL_ERROR "Linker script not found at: ${LINKER_SCRIPT}")
endif()

# Add the freertos_config for FreeRTOS-Kernel
add_library(freertos_config INTERFACE)

#add_library(freertos_boot STATIC)

#target_sources(freertos_boot PRIVATE
#    ${FREERTOS_BOOT_PATH}/start.S
#)


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
# 2. 内核架构编译参数（完全对齐你当前编译指令）
set(CPU_FLAGS
    -mcpu=cortex-r52
#    -mfpu=vfpv3-d16
    -mfpu=fpv5-sp-d16
    -marm
    -mno-thumb-interwork
    -mabi=aapcs-linux
    -mno-unaligned-access
#    -mfloat-abi=soft
    -mthumb 
    -mfloat-abi=hard
    -mlibarch=armv8-r+crc
    -march=armv8-r+crc
)
# 针对 ARM Cortex-R52 (示例，具体取决于你的芯片变体)
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=cortex-r52 -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mcpu=cortex-r52 -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16")
#set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -mcpu=cortex-r52 -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16")

# 3. 通用编译选项
add_compile_options(
    ${CPU_FLAGS}
    -v
    -Og
    -g
    -std=gnu99
    -fno-builtin-printf
    -fomit-frame-pointer
    -fno-strict-aliasing
    -fno-builtin-memcpy
    -Wall
    -Wextra
    # 全局强制32位枚举，彻底消除链接枚举警告
    -fno-short-enums
  #  -Wno-sign-conversion        # 关闭符号转换报错（修复libc.c）
  #  -Wno-error=sign-conversion  # 不把符号转换当错误
    -nostdlib
    -D__FREERTOS__
)

# 4. 链接全局配置
add_link_options(
    ${CPU_FLAGS}
    -fno-short-enums
    # 裸机无系统，补齐系统调用桩，解决_exit/_sbrk/_write等未定义
    # 可自行添加链接脚本
    -nostartfiles
    -nostdlib  -T${LINKER_SCRIPT}
    --specs=nosys.specs

    -Wl,--start-group
    -lgcc
    -lc
    -lm
    -Wl,--end-group

    -Wl,--gc-sections
)

#


# -T${CMAKE_SOURCE_DIR}/link.ld

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

#target_link_options(${PROJECT_NAME}.elf PRIVATE -T${LINKER_SCRIPT})

add_executable(${PROJECT_NAME}.elf
    main.c
    ${FREERTOS_BOOT_PATH}/start.s
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
#target_link_libraries(${PROJECT_NAME}.elf freertos_kernel freertos_config)

#set_property(TARGET freertos_kernel PROPERTY C_STANDARD 90)

# 7. 输出二进制/hex文件
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND arm-none-eabi-objcopy -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
    COMMAND arm-none-eabi-objcopy -O ihex ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex
    COMMENT "Generate bin & hex file"
)
