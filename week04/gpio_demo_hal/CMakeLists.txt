cmake_minimum_required(VERSION 3.22)

#
# STM32F103 GPIO HAL Project
#

# =========================================================
# 1. C LANGUAGE SETTINGS
# =========================================================

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# =========================================================
# 2. PROJECT AND TOOLCHAIN
# =========================================================

set(CMAKE_PROJECT_NAME gpio_demo_hal)

include("${CMAKE_SOURCE_DIR}/cmake/gcc-arm-none-eabi.cmake")

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

project(${CMAKE_PROJECT_NAME})

message(STATUS "Project: ${CMAKE_PROJECT_NAME}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

enable_language(C ASM)

add_executable(${CMAKE_PROJECT_NAME})

# =========================================================
# 3. PREPROCESSOR DEFINITIONS
# =========================================================

set(PROJECT_DEFINES
    STM32F103xB
    USE_HAL_DRIVER
    $<$<CONFIG:Debug>:DEBUG>
)

# =========================================================
# 4. INCLUDE DIRECTORIES
# =========================================================

set(PROJECT_INCLUDE_DIRS
    # Project headers, including stm32f1xx_hal_conf.h
    ${CMAKE_SOURCE_DIR}

    # STM32 HAL headers
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Inc

    # CMSIS Core headers
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Include

    # STM32F1 device headers
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32F1xx/Include
)

# =========================================================
# 5. SOURCE FILES
# =========================================================

set(PROJECT_SOURCES
    # Application
    ${CMAKE_SOURCE_DIR}/main.c

    # Startup and system
    ${CMAKE_SOURCE_DIR}/startup_stm32f103xb.s
    ${CMAKE_SOURCE_DIR}/system_stm32f1xx.c

    # STM32 HAL core
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c

    # STM32 HAL GPIO
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c

    # STM32 HAL Cortex: NVIC and SysTick support
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c
)

# =========================================================
# 6. APPLY SOURCES, INCLUDE PATHS AND DEFINITIONS
# =========================================================

target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCES}
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${PROJECT_INCLUDE_DIRS}
)

target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    ${PROJECT_DEFINES}
)

# =========================================================
# 7. COMPILER OPTIONS
# =========================================================

target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

# =========================================================
# 8. POST-BUILD OUTPUT
# =========================================================

add_custom_command(
    TARGET ${CMAKE_PROJECT_NAME}
    POST_BUILD

    COMMAND ${CMAKE_OBJCOPY}
            -O ihex
            $<TARGET_FILE:${CMAKE_PROJECT_NAME}>
            ${CMAKE_PROJECT_NAME}.hex

    COMMAND ${CMAKE_OBJCOPY}
            -O binary
            $<TARGET_FILE:${CMAKE_PROJECT_NAME}>
            ${CMAKE_PROJECT_NAME}.bin

    COMMENT "Generating HEX and BIN files"
)

add_custom_command(
    TARGET ${CMAKE_PROJECT_NAME}
    POST_BUILD

    COMMAND ${CMAKE_SIZE}
            $<TARGET_FILE:${CMAKE_PROJECT_NAME}>

    COMMENT "Program size:"
)

# =========================================================
# 9. CLEAN GENERATED FILES
# =========================================================

set_target_properties(
    ${CMAKE_PROJECT_NAME}
    PROPERTIES
    ADDITIONAL_CLEAN_FILES
    "${CMAKE_PROJECT_NAME}.map;${CMAKE_PROJECT_NAME}.hex;${CMAKE_PROJECT_NAME}.bin"
)

# =========================================================
# 10. VALIDATE C STANDARD
# =========================================================

if(CMAKE_C_STANDARD LESS 11)
    message(FATAL_ERROR "This project requires C11 or higher")
endif()