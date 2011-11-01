@echo off

cd /d %0\..
echo Switching to %CD% ...
echo Start building and executing test cases.

FOR /D %%G in ("*") DO (
    Echo %%G
    cd %%G
    qmake
    nmake debug
    nmake release
    release\test.exe

    cd ..
)

Echo "Test cases are built and executed."

