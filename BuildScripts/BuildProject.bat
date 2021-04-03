@echo off

set currentDisk=%cd:~0,2%
set currentDir=%cd%

cd /d C:
cd "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\IDE"

call devenv.exe "F:\Users\Documents\SourceControl\Github\C++ Porjects\Reflect\Reflect.sln" /build Debug
call devenv.exe "F:\Users\Documents\SourceControl\Github\C++ Porjects\Reflect\Reflect.sln" /build Release

cd /d %currentDisk%
cd "%currentDir%"
