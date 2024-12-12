@echo off
setlocal enabledelayedexpansion

set target_dir=x64
set flags=/nologo /O2 /Oi /MT /std:c11 /Wall /wd5045 /WX /D _NDEBUG /D UNICODE /D _UNICODE
set sources=..\nl.c

set root="%~dp0"
pushd %root%

if not defined DevEnvDir (
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 (
    call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
)
if errorlevel 1 (
    echo vcvars64.bat not found
    (goto fail)
)
)

rmdir /s /q %target_dir%
mkdir %target_dir%
pushd %target_dir%

cl %flags% /Tc %sources% 

:fail
popd
popd

exit /b errorlevel