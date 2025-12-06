#!/bin/bash

# Wrapper script to launch the PowerShell build script on Linux/macOS

# Check if pwsh (PowerShell Core) is available
if ! command -v pwsh &> /dev/null; then
    echo "Error: PowerShell is not installed on this system."
    echo "Please install PowerShell Core 7+ from: https://github.com/PowerShell/PowerShell"
    exit 1
fi

# Launch the PowerShell build script
exec pwsh -NoProfile -File "$(cd "$(dirname "$0")" && pwd)/build-vs2002.ps1"
