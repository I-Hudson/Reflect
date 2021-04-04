set /p version="Eneter version number or enter to continue: "

SET PATH=%PATH%;"C:\Program Files\7-Zip"
set exeLocation=%cd%\..\bin/Release-windows-x86_64\ReflectEXE
set tempZipFolderName=tempZipFolder

call "BuildProject.bat"


xcopy /i "%exeLocation%\*.*" "%cd%/%tempZipFolderName%/"

IF "%version%" == "" (
    7z a BuiltZips/Reflect_EXE.zip "%cd%/%tempZipFolderName%/*"
) ELSE (
    7z a BuiltZips/Reflect_EXE_%version%.zip "%cd%/%tempZipFolderName%/*"
)

del /f /s /q "%cd%/tempZipFolder" 1>nul
RMDIR /s /q "%cd%/tempZipFolder"