@echo off

set version=%~1

if "%version%" == "" (
    set /p version="Enter version number or enter to continue: "
)

SET PATH=%PATH%;"C:\Program Files\7-Zip"
set headerFiles=%cd%\..\Reflect\inc
set libraryFilesDebug=%cd%\..\bin\Debug-windows-x86_64\Reflect
set libraryFilesRelease=%cd%\..\bin/Release-windows-x86_64\Reflect
set tempZipFolderName=tempZipFolder

echo "%headerFiles%"
echo "%libraryFilesDebug%"
echo "%libraryFilesDebug%"

xcopy /s /e /y /i "%headerFiles%\*.*" "%cd%/%tempZipFolderName%%/inc/"
xcopy /i /y "%libraryFilesDebug%\*.*" "%cd%/%tempZipFolderName%/lib/x64/Debug/*.lib"
xcopy /i /y "%libraryFilesRelease%\*.*" "%cd%/%tempZipFolderName%/lib/x64/Release/*.lib"

set zipArchive=""
IF "%version%" == "" (
    set zipArchive="%cd%\BuiltZips\Reflect_Lib.zip"
) ELSE (
    set zipArchive="%cd%\BuiltZips\Reflect_Lib%version%.zip"
)

echo Building zip with version number %version%
if exist %zipArchive% (
    echo Removing already existing zip
    del /q %zipArchive%
)
7z a %zipArchive% "%cd%/%tempZipFolderName%/*"

del /f /s /q "%cd%/tempZipFolder" 1>nul
RMDIR /s /q "%cd%/tempZipFolder"