@echo off
rem ===========================================================================
rem  Build ONE C++ file with MSVC + OpenCV (D:\opencv\opencv, x64/vc16 prebuilt)
rem  Usage: tools\cl_build.cmd "path\to\your.cpp"
rem  Output: <project folder>\build\<file name>.exe (+ opencv_world DLL copied)
rem  Why MSVC: the prebuilt vc16 .lib files cannot be linked by MinGW g++.
rem ===========================================================================
setlocal enableextensions

set "SRC=%~1"
if "%SRC%"=="" (
    echo [ERROR] Usage: tools\cl_build.cmd "path\to\your.cpp"
    exit /b 1
)
if not exist "%SRC%" (
    echo [ERROR] Source file not found: %SRC%
    exit /b 1
)

set "OCV=D:\opencv\opencv\build"
if not exist "%OCV%\include\opencv2\opencv.hpp" (
    echo [ERROR] OpenCV not found at "%OCV%".
    exit /b 1
)

set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%VSWHERE%" (
    echo [ERROR] vswhere.exe not found. Visual Studio seems incomplete.
    exit /b 1
)

set "VSPATH="
for /f "usebackq tokens=*" %%i in (`"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set "VSPATH=%%i"
if not defined VSPATH (
    echo [ERROR] No Visual Studio installation with the C++ toolset was found.
    exit /b 1
)

call "%VSPATH%\VC\Auxiliary\Build\vcvars64.bat" >nul
if errorlevel 1 (
    echo [ERROR] Failed to initialize the MSVC environment ^(vcvars64.bat^).
    exit /b 1
)

for %%I in ("%~dp0..") do set "PROJECT_DIR=%%~fI"
set "OUTDIR=%PROJECT_DIR%\build"
if not exist "%OUTDIR%" mkdir "%OUTDIR%" >nul 2>&1
set "NAME=%~n1"

rem 自动注入 cv_helper.h：提供 CVT("中文") 转码和控制台 UTF-8，源码里不用 #include
set "HELPER=%~dp0cv_helper.h"

echo [1/2] Compiling %NAME%.cpp with MSVC ...
cl /nologo /std:c++17 /EHsc /MD /O2 /Zi /utf-8 /FI"%HELPER%" /I "%OCV%\include" /Fo"%OUTDIR%\\" /Fd"%OUTDIR%\vc140.pdb" "%SRC%" /link /LIBPATH:"%OCV%\x64\vc16\lib" opencv_world4140.lib /OUT:"%OUTDIR%\%NAME%.exe"
if errorlevel 1 (
    echo.
    echo [ERROR] Compile or link failed. See the errors above.
    exit /b 1
)

echo [2/2] Copying OpenCV runtime and images next to the exe ...
copy /y "%OCV%\x64\vc16\bin\opencv_world4140.dll" "%OUTDIR%" >nul
if errorlevel 1 (
    echo [ERROR] Failed to copy opencv_world4140.dll.
    exit /b 1
)

rem 把项目里的图片也复制到 exe 旁边：这样在 build 目录双击 exe 也能用相对路径读到图
for %%F in ("%PROJECT_DIR%\*.png" "%PROJECT_DIR%\*.jpg" "%PROJECT_DIR%\*.jpeg" "%PROJECT_DIR%\*.bmp") do (
    if exist "%%F" copy /y "%%F" "%OUTDIR%" >nul
)

echo.
echo [OK] %OUTDIR%\%NAME%.exe
exit /b 0
