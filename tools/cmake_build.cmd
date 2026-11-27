@echo off
rem ===========================================================================
rem  CMake build helper for the open project
rem  Usage:  tools\cmake_build.cmd  [Release | Debug | RelWithDebInfo]
rem  Does:   load MSVC env -> CMake configure (Ninja) -> build -> copy DLL/images
rem ===========================================================================
setlocal enableextensions

set "CONFIG=%~1"
if "%CONFIG%"=="" set "CONFIG=Release"

for %%I in ("%~dp0..") do set "PROJECT_DIR=%%~fI"
pushd "%PROJECT_DIR%" || exit /b 1

set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%VSWHERE%" (
    echo [ERROR] vswhere.exe not found. Visual Studio seems incomplete.
    popd & exit /b 1
)

set "VSPATH="
for /f "usebackq tokens=*" %%i in (`"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set "VSPATH=%%i"
if not defined VSPATH (
    echo [ERROR] No Visual Studio installation with the C++ toolset was found.
    popd & exit /b 1
)

call "%VSPATH%\VC\Auxiliary\Build\vcvars64.bat" >nul
if errorlevel 1 (
    echo [ERROR] Failed to initialize the MSVC environment ^(vcvars64.bat^).
    popd & exit /b 1
)

set "BUILD_DIR=%PROJECT_DIR%\build_cmake"

echo [1/2] CMake configure ^(%CONFIG%^) ...
cmake -S "%PROJECT_DIR%" -B "%BUILD_DIR%" -G Ninja -DCMAKE_BUILD_TYPE=%CONFIG% -DOpenCV_DIR="D:/opencv/opencv/build/x64/vc16/lib"
if errorlevel 1 (
    echo.
    echo [ERROR] CMake configure failed. See the first error above.
    popd & exit /b 1
)

echo [2/2] Build ...
cmake --build "%BUILD_DIR%"
if errorlevel 1 (
    echo.
    echo [ERROR] Build failed. See the compiler errors above.
    popd & exit /b 1
)

echo.
echo [OK] Executables in: %BUILD_DIR%
echo      ColorBandLocator.exe
echo      text.exe
popd
exit /b 0
