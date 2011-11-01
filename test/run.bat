@echo off

cd /d %0\..
echo Switching to %CD% ...
echo Building test cases.

FOR /D %%G in ("*") DO (
    Echo %%G
    cd %%G
    qmake
    nmake debug
    nmake release
    cd ..
)

echo Executing test cases.

FOR /D %%G in ("*") DO (
    Echo %%G
    %%G\release\test.exe
)

Echo "Test cases are built and executed."

