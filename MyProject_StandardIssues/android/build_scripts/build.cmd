@echo off
setlocal enabledelayedexpansion

cd ..

REM set NDK_MODULE_PATH=.

set /a CONCURRENT=%NUMBER_OF_PROCESSORS% / 2

call ndk-build --jobs=%CONCURRENT% BUILD_MODE=%1 UNITY_BUILD=%2
if ERRORLEVEL 1 (
	exit /b 1
)

rem call ant release install
call ant debug install

cd build_scripts