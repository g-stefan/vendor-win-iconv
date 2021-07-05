@echo off
rem Public domain
rem http://unlicense.org/
rem Created by Grigore Stefan <g_stefan@yahoo.com>

set ACTION=%1
if "%1" == "" set ACTION=make

echo -^> %ACTION% vendor-win-iconv

goto StepX
:cmdX
%*
if errorlevel 1 goto cmdXError
goto :eof
:cmdXError
echo "Error: %ACTION%"
exit 1
:StepX

call :cmdX xyo-cc --mode=%ACTION% --source-has-archive win-iconv

if not exist include\ mkdir include
if not exist include\iconv.h copy source\iconv.h include\iconv.h
if not exist include\iconv mkdir include\iconv
if not exist include\iconv\iconv.h copy source\iconv.h include\iconv\iconv.h

call :cmdX xyo-cc --mode=%ACTION% @build/source/win-iconv.static.compile
call :cmdX xyo-cc --mode=%ACTION% @build/source/win-iconv.dynamic.compile
call :cmdX xyo-cc --mode=%ACTION% @build/source/iconv.compile
