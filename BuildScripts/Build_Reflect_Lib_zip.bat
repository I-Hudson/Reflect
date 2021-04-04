@echo off

set /p version="Eneter version number or enter to continue: "

SET PATH=%PATH%;"C:\Program Files\7-Zip"
set headerFiles=%cd%\..\Reflect\inc
set libraryFilesDebug=%cd%\..\bin\Debug-windows-x86_64\Reflect
set libraryFilesRelease=%cd%\..\bin/Release-windows-x86_64\Reflect
set tempZipFolderName=tempZipFolder

echo "%headerFiles%"
echo "%libraryFilesDebug%"
echo "%libraryFilesDebug%"

xcopy /s /e /y /i "%headerFiles%\*.*" "%cd%/%tempZipFolderName%%/inc/"
xcopy /i "%libraryFilesDebug%\*.*" "%cd%/%tempZipFolderName%/lib/x64/Debug"
xcopy /i "%libraryFilesRelease%\*.*" "%cd%/%tempZipFolderName%/lib/x64/Release"

IF "%version%" == "" (
    7z a BuiltZips/Reflect_Lib.zip "%cd%/%tempZipFolderName%/*"
) ELSE (
    7z a BuiltZips/Reflect_Lib_%version%.zip "%cd%/%tempZipFolderName%/*"
)

del /f /s /q "%cd%/tempZipFolder" 1>nul
RMDIR /s /q "%cd%/tempZipFolder"