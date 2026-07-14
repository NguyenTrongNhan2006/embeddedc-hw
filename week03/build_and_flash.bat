@echo off
setlocal

echo ==================================================
echo STEP 1: CLEANING BUILD DIRECTORY
echo ==================================================

if exist build (
    echo Deleting existing build folder...
    rmdir /s /q build

    if exist build (
        echo ERROR: Failed to delete build directory.
        goto :error
    )
) else (
    echo Build folder does not exist. Nothing to delete.
)

echo.
echo ==================================================
echo STEP 2: CONFIGURING PROJECT WITH CMAKE
echo ==================================================

cmake -S . -B build -G Ninja

if errorlevel 1 (
    echo ERROR: CMake configuration failed.
    goto :error
)

echo.
echo ==================================================
echo STEP 3: COMPILING FIRMWARE WITH NINJA
echo ==================================================

ninja -C build

if errorlevel 1 (
    echo ERROR: Firmware build failed.
    goto :error
)

if not exist "build\app_firmware.bin" (
    echo ERROR: build\app_firmware.bin was not generated.
    goto :error
)

echo.
echo ==================================================
echo STEP 4: FLASHING FIRMWARE TO TARGET MCU
echo ==================================================

STM32_Programmer_CLI.exe ^
    -c port=SWD ^
    -w "build\app_firmware.bin" 0x08000000 ^
    -v ^
    -rst

if errorlevel 1 (
    echo ERROR: Flashing firmware failed.
    goto :error
)

echo.
echo ==================================================
echo BUILD AND FLASH COMPLETED SUCCESSFULLY
echo ==================================================
echo Firmware file: build\app_firmware.bin
echo Flash address: 0x08000000
echo MCU has been reset.
echo.

pause
exit /b 0

:error
echo.
echo ==================================================
echo BUILD AND FLASH PROCESS FAILED
echo ==================================================
echo Please read the error message above.
echo.

pause
exit /b 1