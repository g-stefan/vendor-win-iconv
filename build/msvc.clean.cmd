@echo off
rem Public domain
rem http://unlicense.org/
rem Created by Grigore Stefan <g_stefan@yahoo.com>

echo -^> clean vendor-win-iconv

if exist output\ rmdir /Q /S output
if exist include\ rmdir /Q /S include

if exist source\ rmdir /Q /S source
if exist temp\ rmdir /Q /S temp
