@echo off

REM Check installation path
set NmapSI4SourcePath=C:\Works\nmapsi4
set NmapSI4BuildPath=C:\Works\nmapsi4\build
set MinGWPath=C:\Qt\2010.05\mingw\bin
REM Set Release or Debug
set BuildType=Release

REM From here you should not change anything
echo Setting up a MinGW/Qt only environment...
echo -- QTDIR set to C:\Qt\2010.05\qt
echo -- PATH set to C:\Qt\2010.05\qt\bin
echo -- Adding C:\Qt\2010.05\bin to PATH
echo -- Adding MinGW to PATH
echo -- Adding Qmake to PATH
echo -- Adding CMake to PATH
echo -- Adding %SystemRoot%\System32 to PATH
echo -- QMAKESPEC set to win32-g++
set QTDIR=C:\Qt\2010.05\qt
set PATH=C:\Qt\2010.05\qt\bin
set PATH=%PATH%;C:\Qt\2010.05\bin;C:\Qt\2010.05\mingw\bin;"C:\Programmi\CMake 2.8\bin"
set PATH=%PATH%;C:\Qt\2010.05\qt\qmake;C:\Qt\2010.05\qt\bin;C:\Qt\2010.05\bin;%MinGWPath%
set PATH=%PATH%;%SystemRoot%\System32
set QMAKESPEC=win32-g++

cd %NmapSI4BuildPath%
echo.
echo Configuring and generating build files
echo.
cmake.exe -G"MinGW Makefiles" -H"%NmapSI4SourcePath%" -B"%NmapSI4BuildPath%" -D"CMAKE_BUILD_TYPE=%BuildType%" -D"CMAKE_INSTALL_PREFIX=%PROGRAM_FILES%\NmapSI4" ../
echo.
echo Building NmapSI4
echo.
%MinGWPath%\mingw32-make.exe

@echo on