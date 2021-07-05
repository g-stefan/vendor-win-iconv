@echo off
rem Public domain
rem http://unlicense.org/
rem Created by Grigore Stefan <g_stefan@yahoo.com>

echo -^> sign vendor-win-iconv

pushd output
for /r %%i in (*.dll) do call grigore-stefan.sign "Win-iconv" "%%i"
for /r %%i in (*.exe) do call grigore-stefan.sign "Win-iconv" "%%i"
popd
