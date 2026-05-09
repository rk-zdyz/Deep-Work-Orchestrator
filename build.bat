@echo off
echo ========================================
echo Building Deep Work Orchestrator...
echo ========================================

REM Set up the Visual Studio build environment
call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" x64 >nul 2>&1

REM /EHsc enables standard C++ error handling
REM /link /MANIFEST:EMBED embeds our app.manifest into the exe
REM /OUT names the final file

cl.exe /EHsc /I include src\main.cpp src\ProcessManager.cpp src\NetworkSinkHole.cpp src\EnvironmentLauncher.cpp /link shell32.lib /MANIFESTUAC:NO /MANIFEST:EMBED /MANIFESTINPUT:app.manifest /OUT:orchestrator.exe

if %errorlevel% == 0 (
    echo.
    echo [SUCCESS] orchestrator.exe has been built!
) else (
    echo.
    echo [ERROR] Build failed. 
    echo Make sure you are running this from the "Developer Command Prompt for VS"
)

pause