@echo off
rem Public domain
rem http://unlicense.org/
rem Created by Grigore Stefan <g_stefan@yahoo.com>

SETLOCAL ENABLEDELAYEDEXPANSION

echo -^> git-release-check vendor-win-iconv

set PROJECT=vendor-win-iconv
SET PROJECT_VENDOR=%PROJECT:vendor-=%
if not exist %PROJECT_VENDOR%.version.ini echo "Error - no version info" & exit 1
FOR /F "tokens=* USEBACKQ" %%F IN (`xyo-version --no-bump --get "--version-file=%PROJECT_VENDOR%.version.ini" %PROJECT_VENDOR%`) DO (
	SET VERSION=%%F
)

github-release info --repo %PROJECT% --tag v%VERSION%

