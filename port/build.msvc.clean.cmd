@echo off
rem Public domain
rem http://unlicense.org/
rem Created by Grigore Stefan <g_stefan@yahoo.com>

echo -^> clean vendor-win-iconv

if exist bin\ rmdir /Q /S bin
if exist include\ rmdir /Q /S include
if exist lib\ rmdir /Q /S lib
if exist source\ rmdir /Q /S source
if exist build\ rmdir /Q /S build
