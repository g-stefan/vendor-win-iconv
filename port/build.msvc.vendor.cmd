@echo off
rem Public domain
rem http://unlicense.org/
rem Created by Grigore Stefan <g_stefan@yahoo.com>

echo -^> vendor vendor-win-iconv

if not exist archive\ mkdir archive

pushd archive
set VENDOR=win-iconv-0.0.8
set WEB_LINK=https://github.com/win-iconv/win-iconv/archive/v0.0.8.zip
if not exist %VENDOR%.zip curl --insecure --location %WEB_LINK% --output %VENDOR%.zip
7z x %VENDOR%.zip -aoa -o.
del /F /Q %VENDOR%.zip
7zr a -mx9 -mmt4 -r- -sse -w. -y -t7z %VENDOR%.7z %VENDOR%
rmdir /Q /S %VENDOR%
popd
