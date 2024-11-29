@echo off

set version=%~1

if "%version%" == "" (
    set /p version="Enter version number or enter to continue: "
)

SET PATH=%PATH%;"C:\Program Files\7-Zip"
set exeLocation=%cd%\..\bin/Release-windows-x86_64\ReflectEXE
set tempZipFolderName=tempZipFolder

if exist "%cd%/%tempZipFolderName%/" (
    echo Removing already existing zip temp folder
    del /q "%tempZipFolderName%"
)

call "BuildProject.bat"

echo Copying files from "%exeLocation%" to "%cd%/%tempZipFolderName%/"
xcopy /i /y "%exeLocation%\*.exe" "%cd%/%tempZipFolderName%/"

cd "%cd%/%tempZipFolderName%"

echo Renaming exe to Reflect.exe
ren ReflectEXE.exe Reflect.exe

cd "../"

set zipArchive=""
IF "%version%" == "" (
    set zipArchive="%cd%\BuiltZips\Reflect_EXE.zip"
) ELSE (
    set zipArchive="%cd%\BuiltZips\Reflect_EXE_%version%.zip"
)

echo Building zip with version number %version%
if exist %zipArchive% (
    echo Removing already existing zip
    del /q %zipArchive%
)
7z a %zipArchive% "%cd%/%tempZipFolderName%/*"

del /f /s /q "%cd%/tempZipFolder" 1>nul
RMDIR /s /q "%cd%/tempZipFolder"