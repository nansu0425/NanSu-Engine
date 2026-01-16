@echo off
echo Generating Visual Studio 2022 project files...
echo.

call premake5.exe vs2022

if %errorlevel% neq 0 (
    echo.
    echo Error: Failed to generate project files!
    pause
    exit /b %errorlevel%
)

echo.
echo Project files generated successfully!
echo.
pause
