@echo off

set VS_2022_Preview=C:\Program Files\Microsoft Visual Studio\2022\Preview\Common7\IDE
set VS_2022_Community=C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE
set VS_2019_Community=C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\IDE

set VSDir=""
if exist %VS_2022_Preview% (
	echo Using VS 2022 Preview
	set VSDir=%VS_2022_Preview%
	GOTO BUILD
)
if exist %VS_2022_Preview% (
	echo Using VS 2022 Community
	set VSDir=%VS_2022_Community%
	GOTO BUILD
)
if exist %VS_2022_Preview% (
	echo Using VS 2019 Community
	set VSDir=%VS_2019_Community%
	GOTO BUILD
)
GOTO END

:BUILD
echo Building Debug Windows x64
call "%VSDir%\devenv.exe" "..\Reflect.sln" /build Debug
echo Building Release Windows x64
call "%VSDir%\devenv.exe" "..\Reflect.sln" /build Release

echo Finshed building
GOTO END

:END
