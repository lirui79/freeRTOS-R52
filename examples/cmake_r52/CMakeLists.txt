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


# 设置目标环境路径（库和头文件）
#set(CMAKE_FIND_ROOT_PATH /path/to/sysroot)
set(FREERTOS_KERNEL_PATH "../../")
# 配置查找模式
#set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
#set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
#set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)


# Add the freertos_config for FreeRTOS-Kernel
add_library(freertos_config INTERFACE)

target_include_directories(freertos_config
    INTERFACE
    "../configuration"
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
    -mfpu=vfpv3-d16
    -marm
    -mno-thumb-interwork
    -mabi=aapcs-linux
    -mno-unaligned-access
    -mfloat-abi=soft
    -mlibarch=armv8-r+crc
    -march=armv8-r+crc
)

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
    -nostdlib
)

# 4. 链接全局配置
add_link_options(
    ${CPU_FLAGS}
    -fno-short-enums
    # 裸机无系统，补齐系统调用桩，解决_exit/_sbrk/_write等未定义
    --specs=nosys.specs
    # 可自行添加链接脚本
     -nostdlib -T${CMAKE_SOURCE_DIR}/link.ld
)


#add_compile_options( -v -Og -g -std=gnu99 -mcpu=cortex-r52 -mfpu=vfpv3-d16 )
#add_compile_options( -marm -mno-thumb-interwork -mabi=aapcs-linux -mno-unaligned-access )
#add_compile_options( -fno-builtin-printf -fomit-frame-pointer -fno-strict-aliasing -fno-builtin-memcpy )
#add_compile_options( -Wall -Wextra -fno-short-enums )
# ========== 一键解决你所有错误 ==========
# 1. 强制 32位枚举（消除枚举警告）
#add_compile_options( -fno-short-enums -nostdlib )
#add_link_options( -fno-short-enums -nostdlib )

# 2. 裸机链接配置（消除 _exit/_close/_sbrk 未定义错误）
#add_link_options( --specs=nosys.specs )



# Select the heap port.  values between 1-4 will pick a heap.
set(FREERTOS_HEAP "4" CACHE STRING "" FORCE)

# Select the native compile PORT
#set(FREERTOS_PORT "TEMPLATE" CACHE STRING "" FORCE)
set(FREERTOS_PORT "GCC_ARM_CR52" CACHE STRING "" FORCE)

# Adding the FreeRTOS-Kernel subdirectory
add_subdirectory(${FREERTOS_KERNEL_PATH} FreeRTOS-Kernel)

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

#add_executable(${PROJECT_NAME}
#    main.c
#)

#target_link_libraries(${PROJECT_NAME} freertos_kernel freertos_config)

#set_property(TARGET freertos_kernel PROPERTY C_STANDARD 90)

# 5. 源文件目录
set(SRC_DIR ${CMAKE_SOURCE_DIR})
set(FREERTOS_DIR ${CMAKE_SOURCE_DIR}/../../)

# 业务源码
file(GLOB MAIN_SRCS
    ${SRC_DIR}/*.c
    ${SRC_DIR}/*.S
)

# FreeRTOS 内核源码
file(GLOB FREERTOS_SRCS
    ${FREERTOS_DIR}/*.c
    ${FREERTOS_DIR}/libs/*.c
    ${FREERTOS_DIR}/portable/GCC/ARM_CR52/*.c
    ${FREERTOS_DIR}/portable/MemMang/*_4.c
)

# 汇总所有源码
add_executable(${PROJECT_NAME}.elf
    ${MAIN_SRCS}
    ${FREERTOS_SRCS}
)

# 6. 头文件路径
target_include_directories(${PROJECT_NAME}.elf PRIVATE
    ${SRC_DIR}
    ${SRC_DIR}/../configuration
    ${FREERTOS_DIR}/include
    ${FREERTOS_DIR}/portable/GCC/ARM_CR52/
    # 自行添加其他驱动/外设头文件
)

# 7. 输出二进制/hex文件
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND arm-none-eabi-objcopy -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
    COMMAND arm-none-eabi-objcopy -O ihex ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex
    COMMENT "Generate bin & hex file"
)