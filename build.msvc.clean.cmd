@echo off
rem Public domain
rem http://unlicense.org/
rem Created by Grigore Stefan <g_stefan@yahoo.com>

echo -^> clean vendor-win-iconv

if exist bin\ rmdir /Q /S bin
if exist build\ rmdir /Q /S build
if exist lib\ rmdir /Q /S lib

if exist win-iconv-0.0.8\ rmdir /Q /S win-iconv-0.0.8

