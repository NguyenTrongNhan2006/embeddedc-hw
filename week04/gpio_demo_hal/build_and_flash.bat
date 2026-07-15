@echo off
setlocal

echo ===============================================
echo Building and Flashing GPIO Demo HAL Project
echo ===============================================

REM Chuyen den thu muc chua file BAT
cd /d "%~dp0"

REM Kiem tra CMakeLists.txt
if not exist "CMakeLists.txt" (
    echo.
    echo ERROR: Khong tim thay CMakeLists.txt tai:
    echo %CD%
    pause
    exit /b 1
)

REM Xoa thu muc build cu
if exist "build" (
    echo Cleaning build directory...
    rmdir /s /q "build"
)

REM Tao thu muc build
mkdir "build"

REM Configure project
echo.
echo [1/3] Configuring...
cmake -B build -G Ninja

if errorlevel 1 (
    echo.
    echo Configuration failed!
    pause
    exit /b 1
)

REM Build project
echo.
echo [2/3] Building...
cmake --build build

if errorlevel 1 (
    echo.
    echo Build failed!
    pause
    exit /b 1
)

REM Hien thi ket qua build
echo.
echo [3/3] Build Summary
echo ===============================================

if exist "build\gpio_demo_hal.hex" (
    dir "build\gpio_demo_hal.hex"
) else (
    echo WARNING: No gpio_demo_hal.hex generated!
)

if exist "build\gpio_demo_hal.bin" (
    dir "build\gpio_demo_hal.bin"
) else (
    echo.
    echo ERROR: File build\gpio_demo_hal.bin does not exist!
    pause
    exit /b 1
)

echo ===============================================
echo Build completed successfully!
echo.

REM Flash file BIN vao STM32
echo Flashing gpio_demo_hal.bin...
STM32_Programmer_CLI ^
-c port=SWD sn=31FF6B064E55353255471543 ^
-w "build\gpio_demo_hal.bin" 0x08000000 ^
-v ^
-rst

if errorlevel 1 (
    echo.
    echo Flash failed!
    pause
    exit /b 1
)

echo.
echo ===============================================
echo FLASH COMPLETED SUCCESSFULLY!
echo ===============================================

pause
endlocal