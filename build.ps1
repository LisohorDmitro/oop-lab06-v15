param([string]$CompilerDirectory = "")
$ErrorActionPreference = "Stop"
if ($CompilerDirectory) {
    $bin = (Resolve-Path -LiteralPath $CompilerDirectory).Path
    $env:PATH = $bin + [IO.Path]::PathSeparator + $env:PATH
}
$g++ = (Get-Command g++ -CommandType Application -ErrorAction Stop | Select-Object -First 1).Source
Push-Location -LiteralPath $PSScriptRoot
try {
    & $g++ '-std=c++14' '-O2' '-Wall' '-Wextra' '-pedantic' '-DFRACTION_DLL' '-DFRACTION_EXPORTS' '-c' 'fraction.cpp' '-o' 'fraction.o'
    if ($LASTEXITCODE -ne 0) { throw 'g++ failed' }
    & $g++ '-shared' 'fraction.o' '-Wl,--out-implib,libfraction.dll.a' '-o' 'fraction.dll'
    if ($LASTEXITCODE -ne 0) { throw 'g++ failed' }
    & $g++ '-std=c++14' '-O2' '-Wall' '-Wextra' '-pedantic' '-DFRACTION_DLL' 'main.cpp' '-L.' '-lfraction' '-o' 'lab06.exe'
    if ($LASTEXITCODE -ne 0) { throw 'g++ failed' }
} finally {
    Pop-Location
}
