@echo off
REM Wrapper batch script to launch the PowerShell build script on Windows

REM Check if PowerShell is available
where pwsh >nul 2>&1
if %ERRORLEVEL% equ 0 (
    REM PowerShell Core 7+ is available
    pwsh -NoProfile -File "%~dp0build-vs2002.ps1"
) else (
    REM Try Windows PowerShell 5.1 (built-in on Windows 10/11)
    where powershell >nul 2>&1
    if %ERRORLEVEL% equ 0 (
        powershell -NoProfile -File "%~dp0build-vs2002.ps1"
    ) else (
        echo Error: PowerShell is not found on this system.
        echo Please install PowerShell or ensure it is in your PATH.
        exit /b 1
    )
)
