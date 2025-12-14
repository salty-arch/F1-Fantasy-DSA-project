# PowerShell-friendly build script for the project
param(
    [string]$OutputDir = "build",
    [string]$OutputExe = "main.exe"
)

# Ensure an absolute output directory exists under the script root
$outDirFull = Join-Path $scriptDir $OutputDir
if (-not (Test-Path -Path $outDirFull)) {
    New-Item -ItemType Directory -Path $outDirFull -Force | Out-Null
}

# Quote the -std flag to avoid PowerShell parsing issues

$stdFlag = '-std=c++17'

Write-Host "Building project..."

# Use the script directory as the project root so runs from any cwd work
$scriptDir = $PSScriptRoot

# Gather all .cpp files under src explicitly so wildcard expansion is correct
$srcDir = Join-Path $scriptDir 'src'
$srcFiles = Get-ChildItem -Path $srcDir -Filter '*.cpp' -File | ForEach-Object { $_.FullName }

if ($srcFiles.Count -eq 0) {
    Write-Error "No .cpp files found under $srcDir"
    exit 1
}

$outPath = Join-Path $outDirFull $OutputExe

& g++ $stdFlag $srcFiles -I $srcDir -o $outPath

if ($LASTEXITCODE -eq 0) {
    Write-Host "Build succeeded -> $OutputDir\$OutputExe"
    exit 0
} else {
    Write-Error "Build failed with exit code $LASTEXITCODE"
    exit $LASTEXITCODE
}
