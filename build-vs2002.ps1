$toolchainDownloadURL = "https://storage.alula.me/dev/toolchains/MSVC70.zip"
$toolchainHash = "cc3568d964d74106888d206e7bc2a5e5fa6f1a8944d585a39d84158930b2891f"

$cmakeDownloadURL = "https://github.com/Kitware/CMake/releases/download/v4.2.0/cmake-4.2.0-windows-x86_64.zip"
$cmakeHash = "cf35a516c4f5f4646b301e51c8e24b168cc012c3b1453b8f675303b54eb0ef45"

$ninjaDownloadURL = "https://github.com/ninja-build/ninja/releases/download/v1.13.2/ninja-win.zip"
$ninjaHash = "07fc8261b42b20e71d1720b39068c2e14ffcee6396b76fb7a795fb460b78dc65"

# Check if running from project root
$rootIndicators = @("CMakeLists.txt", "source", "CMakeSettings.json")
$missingIndicators = $rootIndicators | Where-Object { -not (Test-Path $_) }

if ($missingIndicators) {
    Write-Error "Error: This script must be run from the project root directory."
    Write-Error "Current directory '$PWD' is missing: $($missingIndicators -join ', ')"
    exit 1
}

# Check if system is 64-bit
if ([Environment]::Is64BitOperatingSystem -eq $false) {
    Write-Error "Error: This script requires a 64-bit system. CMake and Ninja are only available for Windows x64."
    exit 1
}

$toolsDir = Join-Path $PWD "_tools"
if (-not (Test-Path $toolsDir)) {
    New-Item -ItemType Directory -Path $toolsDir | Out-Null
}

# Determine platform and wrapper
# We rely solely on $IsWindows (available in PowerShell Core/6+).
# If running on Windows PowerShell 5.1, this might be $null/false, so ensure you are using a modern PowerShell or adjust if needed.
$runningOnWindows = $IsWindows
$wineKeeper = $null

if (-not $runningOnWindows) {
    if (-not (Get-Command "wine" -ErrorAction SilentlyContinue)) {
        Write-Error "Error: 'wine' is required on non-Windows platforms."
        exit 1
    }

    # Start a background Wine process to keep wineserver alive
    # This significantly speeds up subsequent wine/winepath calls
    # Write-Host "Starting background Wine process to keep wineserver alive..."
    $wineKeeper = Start-Process -FilePath "wine" -ArgumentList "cmd", "/c", "timeout", "/t", "3600" -PassThru -RedirectStandardOutput "/dev/null"
    & wineserver -p30
    Start-Sleep -s 2
}

try {
    # Helper function to download and extract
    function Install-Tool {
    param (
        [string]$Name,
        [string]$Url,
        [string]$Hash,
        [string]$DestCheck,
        [string]$ExtractDir,
        [scriptblock]$PostAction
    )

    if (Test-Path $DestCheck) {
        Write-Host "$Name is already installed."
        return
    }

    Write-Host "Downloading $Name..."
    $zipPath = Join-Path $toolsDir "$Name.zip"
    try {
        Invoke-WebRequest -Uri $Url -OutFile $zipPath
    } catch {
        Write-Error "Failed to download $Name from $Url"
        exit 1
    }

    if ($Hash) {
        Write-Host "Verifying $Name hash..."
        $fileHash = Get-FileHash -Path $zipPath -Algorithm SHA256
        if ($fileHash.Hash -ne $Hash) {
            Write-Error "Hash mismatch for $Name. Expected $Hash, got $($fileHash.Hash)"
            Remove-Item $zipPath
            exit 1
        }
    }

    Write-Host "Extracting $Name..."
    Expand-Archive -Path $zipPath -DestinationPath $ExtractDir -Force
    Remove-Item $zipPath

    if ($PostAction) {
        & $PostAction
    }
}

# Helper function to convert Unix paths to Wine paths
function Convert-ToWinePath {
    param (
        [string]$UnixPath
    )
    
    $result = & winepath -w $UnixPath 2>$null
    if ($LASTEXITCODE -eq 0) {
        # Write-Host "Converted path: $UnixPath -> $result"
        return $result.Trim()
    } else {
        Write-Error "Failed to convert path: $UnixPath"
        exit 1
    }
}

# 1. Install MSVC70
Install-Tool -Name "MSVC70" -Url $toolchainDownloadURL -Hash $toolchainHash `
    -DestCheck (Join-Path $toolsDir "MSVC70") `
    -ExtractDir $toolsDir `
    -PostAction {
        $extracted = Join-Path $toolsDir "Microsoft Visual Studio .NET"
        $target = Join-Path $toolsDir "MSVC70"
        if (Test-Path $extracted) {
            if (Test-Path $target) { Remove-Item -Recurse -Force $target }
            Rename-Item -Path $extracted -NewName "MSVC70"
        }
    }

# 2. Install CMake
Install-Tool -Name "CMake" -Url $cmakeDownloadURL -Hash $cmakeHash `
    -DestCheck (Join-Path $toolsDir "cmake") `
    -ExtractDir $toolsDir `
    -PostAction {
        $extracted = Get-ChildItem -Path $toolsDir -Directory | Where-Object { $_.Name -like "cmake-*-windows-*" } | Select-Object -First 1
        $target = Join-Path $toolsDir "cmake"
        if ($extracted) {
            if (Test-Path $target) { Remove-Item -Recurse -Force $target }
            Rename-Item -Path $extracted.FullName -NewName "cmake"
        }
    }

# 3. Install Ninja
$cmakeBin = Join-Path $toolsDir "cmake/bin"
Install-Tool -Name "Ninja" -Url $ninjaDownloadURL -Hash $ninjaHash `
    -DestCheck (Join-Path $cmakeBin "ninja.exe") `
    -ExtractDir $cmakeBin

# Setup Visual Studio .NET environment (equivalent to vsvars32.bat)
$msvcRoot = Join-Path $toolsDir "MSVC70"
$vcInstallDir = $msvcRoot
$vsInstallDir = Join-Path $msvcRoot "Common7" "IDE"
$devEnvDir = $vsInstallDir
$msvcDir = Join-Path $vcInstallDir "Vc7"
$frameworkSDKDir = Join-Path $msvcRoot "FrameworkSDK"

# Prepare paths for MSVC environment
$msvcBinPath = Join-Path $msvcDir "bin"
$msvcIncludePaths = @(
    (Join-Path $msvcDir "atlmfc" "include"),
    (Join-Path $msvcDir "include"),
    (Join-Path $msvcDir "PlatformSDK" "Include" "prerelease"),
    (Join-Path $msvcDir "PlatformSDK" "Include")
)
$msvcLibPaths = @(
    (Join-Path $msvcDir "atlmfc" "lib"),
    (Join-Path $msvcDir "lib"),
    (Join-Path $msvcDir "PlatformSDK" "lib" "prerelease"),
    (Join-Path $msvcDir "PlatformSDK" "lib"),
    (Join-Path $frameworkSDKDir "lib")
)

$cmakeBinPath = Join-Path $toolsDir "cmake" "bin"
$commonToolsPath = Join-Path $vcInstallDir "Common7" "Tools"

Write-Host "Setting up build environment..."
if ($runningOnWindows) {
    # Windows native setup
    $env:VSINSTALLDIR = $vsInstallDir
    $env:VCINSTALLDIR = $vcInstallDir
    $env:DevEnvDir = $devEnvDir
    $env:MSVCDir = $msvcDir
    $env:FrameworkSDKDir = $frameworkSDKDir

    $pathComponents = @(
        $devEnvDir,
        $msvcBinPath,
        $commonToolsPath,
        (Join-Path $commonToolsPath "Bin"),
        $cmakeBinPath
    )
    
    # Prepend to PATH
    $newPath = $pathComponents + ($env:PATH -split ';')
    $env:PATH = ($newPath | Select-Object -Unique) -join ';'
    
    # Prepend to INCLUDE and LIB
    $env:INCLUDE = ($msvcIncludePaths + ($env:INCLUDE -split ';') | Select-Object -Unique) -join ';'
    $env:LIB = ($msvcLibPaths + ($env:LIB -split ';') | Select-Object -Unique) -join ';'
} else {
    # Wine setup
    $vsInstallDir_Wine = Convert-ToWinePath $vsInstallDir
    $vcInstallDir_Wine = Convert-ToWinePath $vcInstallDir
    $devEnvDir_Wine = Convert-ToWinePath $devEnvDir
    $msvcDir_Wine = Convert-ToWinePath $msvcDir
    $frameworkSDKDir_Wine = Convert-ToWinePath $frameworkSDKDir

    $env:VSINSTALLDIR = $vsInstallDir_Wine
    $env:VCINSTALLDIR = $vcInstallDir_Wine
    $env:DevEnvDir = $devEnvDir_Wine
    $env:MSVCDir = $msvcDir_Wine
    $env:FrameworkSDKDir = $frameworkSDKDir_Wine

    $devEnvDir_Wine = Convert-ToWinePath $devEnvDir
    $msvcBinPath_Wine = Convert-ToWinePath $msvcBinPath
    $commonToolsPath_Wine = Convert-ToWinePath $commonToolsPath
    $commonToolsPathBin_Wine = Convert-ToWinePath (Join-Path $commonToolsPath "Bin")
    $cmakeBinPath_Wine = Convert-ToWinePath $cmakeBinPath
    
    $winePathComponents = @(
        $devEnvDir_Wine,
        $msvcBinPath_Wine,
        $commonToolsPath_Wine,
        $commonToolsPathBin_Wine,
        $cmakeBinPath_Wine
    )
    
    # For Wine, we use WINEPATH instead of PATH
    # Prepend to WINEPATH
    if ($env:WINEPATH) {
        $newWinePath = $winePathComponents + ($env:WINEPATH -split ';')
        $env:WINEPATH = ($newWinePath | Select-Object -Unique) -join ';'
    } else {
        $env:WINEPATH = $winePathComponents -join ';'
    }
    
    # Convert include and lib paths for Wine
    $msvcIncludePaths_Wine = $msvcIncludePaths | ForEach-Object { Convert-ToWinePath $_ }
    $msvcLibPaths_Wine = $msvcLibPaths | ForEach-Object { Convert-ToWinePath $_ }
    
    $env:INCLUDE = ($msvcIncludePaths_Wine + ($env:INCLUDE -split ';') | Select-Object -Unique) -join ';'
    $env:LIB = ($msvcLibPaths_Wine + ($env:LIB -split ';') | Select-Object -Unique) -join ';'
}

# Run CMake
$cmakeExe = Join-Path $cmakeBinPath "cmake.exe"
$cmakeArgs = @("-S", ".", "-B", "build", "-G", "Ninja", "-DCMAKE_BUILD_TYPE=Release")

Write-Host "Configuring project..."
if (-not $runningOnWindows) {
    $cmakeExeWine = Convert-ToWinePath $cmakeExe
    & wine $cmakeExeWine $cmakeArgs
} else {
    & $cmakeExe $cmakeArgs
}

if ($LASTEXITCODE -ne 0) {
    Write-Error "CMake configuration failed."
    exit $LASTEXITCODE
}

Write-Host "Building project..."
$buildArgs = @("--build", "build", "--config", "Release")

if (-not $runningOnWindows) {
    $cmakeExeWine = Convert-ToWinePath $cmakeExe
    & wine $cmakeExeWine $buildArgs
} else {
    & $cmakeExe $buildArgs
}

    if ($LASTEXITCODE -ne 0) {
        Write-Error "Build failed."
        exit $LASTEXITCODE
    }

    Write-Host "Build completed successfully."
} finally {
    if ($wineKeeper -and -not $wineKeeper.HasExited) {
        # Write-Host "Stopping background Wine process..."
        Stop-Process -InputObject $wineKeeper -Force
    }
}