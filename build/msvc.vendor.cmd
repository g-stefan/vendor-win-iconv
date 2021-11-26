@echo off
rem Public domain
rem http://unlicense.org/
rem Created by Grigore Stefan <g_stefan@yahoo.com>

echo - %BUILD_PROJECT% ^> vendor

goto getSizeDefined
:getSize
set FILE_SIZE=%~z1
goto:eof
:getSizeDefined

if not exist archive\ mkdir archive

rem Self
if exist archive\%PROJECT%-%VERSION%.7z goto:eof
curl --insecure --location https://github.com/g-stefan/vendor-%PROJECT%/releases/download/v%VERSION%/%PROJECT%-%VERSION%.7z --output archive\%PROJECT%-%VERSION%.7z
call :getSize "archive\%PROJECT%-%VERSION%.7z"
if %FILE_SIZE% GTR 16 goto:eof
del /F /Q archive\%PROJECT%-%VERSION%.7z goto:eof

rem Source
pushd archive
set VENDOR=%PROJECT%-%VERSION%
set WEB_LINK=https://github.com/win-iconv/win-iconv/archive/v0.0.8.zip
if not exist %VENDOR%.zip curl --insecure --location %WEB_LINK% --output %VENDOR%.zip
7z x %VENDOR%.zip -aoa -o.
del /F /Q %VENDOR%.zip
if exist %VENDOR%.7z del /F /Q %VENDOR%.7z
7zr a -mx9 -mmt4 -r- -sse -w. -y -t7z %VENDOR%.7z %VENDOR%
rmdir /Q /S %VENDOR%
popd
