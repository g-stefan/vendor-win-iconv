@echo off
rem Public domain
rem http://unlicense.org/
rem Created by Grigore Stefan <g_stefan@yahoo.com>

set ACTION=%1
if "%1" == "" set ACTION=make

echo - %BUILD_PROJECT% ^> %1

goto cmdXDefined
:cmdX
%*
if errorlevel 1 goto cmdXError
goto :eof
:cmdXError
echo "Error: %ACTION%"
exit 1
:cmdXDefined

call :cmdX xyo-cc --mode=%ACTION% --source-has-archive win-iconv

if not exist output\include\ mkdir output\include
if not exist output\include\iconv.h copy source\iconv.h output\include\iconv.h
if not exist output\include\iconv mkdir output\include\iconv
if not exist output\include\iconv\iconv.h copy source\iconv.h output\include\iconv\iconv.h

call :cmdX xyo-cc --mode=%ACTION% @build/source/win-iconv.static.compile
call :cmdX xyo-cc --mode=%ACTION% @build/source/win-iconv.dynamic.compile
call :cmdX xyo-cc --mode=%ACTION% @build/source/iconv.compile
