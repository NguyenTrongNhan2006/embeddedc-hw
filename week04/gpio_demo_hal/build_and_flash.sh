#!/usr/bin/env bash
#
# Build and flash STM32F103 GPIO Demo HAL Project (Linux)
#

set -euo pipefail

# Thu muc chua script cung la thu muc goc cua project
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

PROJECT_NAME="gpio_demo_hal"
BUILD_DIR="build"

# Doi serial nay neu Linux dang dung ST-Link khac
STLINK_SN="31FF6B064E55353255471543"

# Cho phep:
# 1. Goi truc tiep neu STM32_Programmer_CLI da nam trong PATH
# 2. Hoac dung STM32_PRG_PATH neu nguoi dung da khai bao
if [ -n "${STM32_PRG_PATH:-}" ]; then
    STM32_PROGRAMMER_CLI="${STM32_PRG_PATH}/STM32_Programmer_CLI"
else
    STM32_PROGRAMMER_CLI="STM32_Programmer_CLI"
fi

echo "==============================================="
echo "Building and Flashing GPIO Demo HAL Project"
echo "==============================================="

# Kiem tra cac cong cu can thiet
if ! command -v arm-none-eabi-gcc >/dev/null 2>&1; then
    echo "Error: arm-none-eabi-gcc not found in PATH."
    echo "Install it with: sudo apt install gcc-arm-none-eabi"
    exit 1
fi

if ! command -v cmake >/dev/null 2>&1; then
    echo "Error: cmake not found in PATH."
    echo "Install it with: sudo apt install cmake"
    exit 1
fi

if ! command -v ninja >/dev/null 2>&1; then
    echo "Error: ninja not found in PATH."
    echo "Install it with: sudo apt install ninja-build"
    exit 1
fi

if ! command -v "$STM32_PROGRAMMER_CLI" >/dev/null 2>&1; then
    echo "Error: STM32_Programmer_CLI not found."
    echo "Add it to PATH or set STM32_PRG_PATH."
    exit 1
fi

# Kiem tra CMakeLists.txt
if [ ! -f "CMakeLists.txt" ]; then
    echo "Error: CMakeLists.txt not found in:"
    echo "$SCRIPT_DIR"
    exit 1
fi

# Xoa build cu
if [ -d "$BUILD_DIR" ]; then
    echo "Cleaning build directory..."
    rm -rf "$BUILD_DIR"
fi

# Configure
echo
echo "[1/3] Configuring..."
cmake -B "$BUILD_DIR" -G Ninja

# Build
echo
echo "[2/3] Building..."
cmake --build "$BUILD_DIR"

# Summary
echo
echo "[3/3] Build Summary"
echo "==============================================="

if compgen -G "$BUILD_DIR/*.hex" > /dev/null; then
    ls -la "$BUILD_DIR"/*.hex
else
    echo "Warning: No .hex file generated!"
fi

if compgen -G "$BUILD_DIR/*.bin" > /dev/null; then
    ls -la "$BUILD_DIR"/*.bin
else
    echo "Error: No .bin file generated!"
    exit 1
fi

BIN_FILE="$BUILD_DIR/${PROJECT_NAME}.bin"

if [ ! -f "$BIN_FILE" ]; then
    echo "Error: File does not exist: $BIN_FILE"
    echo "Available BIN files:"
    find "$BUILD_DIR" -maxdepth 1 -type f -name "*.bin" -print
    exit 1
fi

echo "==============================================="
echo "Build completed successfully!"
echo

# Flash
echo "Flashing $BIN_FILE via ST-Link..."

"$STM32_PROGRAMMER_CLI" \
    -c port=SWD sn="$STLINK_SN" \
    -w "$BIN_FILE" 0x08000000 \
    -v \
    -rst

echo
echo "==============================================="
echo "FLASH COMPLETED SUCCESSFULLY!"
echo "==============================================="