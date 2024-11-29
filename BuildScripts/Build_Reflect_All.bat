@echo off

set /p version="Eneter version number or enter to continue: "

call Build_Reflect_EXE_zip %version%
call Build_Reflect_Lib_zip %version%