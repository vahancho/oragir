@echo off

cd /d %0\..
echo Switching to %CD% ...
echo Building test cases.

FOR /D %%G in ("*") DO (
    Echo %%G
    cd %%G
    qmake
    nmake debug
    if errorlevel 1 goto BuildError
    nmake release
    if errorlevel 1 goto BuildError
    cd ..
)

echo Executing test cases.

FOR /D %%G in ("*") DO (
    Echo %%G
    %%G\release\test.exe
)

Echo Test cases are built and executed.
exit /b

:BuildError
    cd ..
    echo Build error %errorlevel%
    exit /b %errorlevel%
