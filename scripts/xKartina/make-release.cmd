@echo off
rem Script to update version numbers and pack everything together
rem
rem PLEASE NOTE: 
rem     In files where you update the version number all the 
rem     blank lines will be DELETED!
rem     A nice feature of MS find command!
rem
rem Author: consros 2010

set RAR="D:\Program Files\7-Zip\7z.exe"
set TEMP=tmp
set NAME=xKartina
set ZIP=%NAME%

mkdir %TEMP%
mkdir %TEMP%\%NAME%

setLocal
setLocal EnableDelayedExpansion

if "%1" NEQ "-n" call :UPDATE_VERSION version.xml "version"

echo Copying required files:
xcopy /Q /I /E ..\%NAME%\img       %TEMP%\%NAME%\img
xcopy /Q /I /E ..\%NAME%\templates %TEMP%\%NAME%\templates
xcopy /Q /I /E ..\%NAME%\playlists %TEMP%\%NAME%\playlists
xcopy /Q /I ..\%NAME%\*.inc %TEMP%\%NAME%\
xcopy /Q /I ..\%NAME%\*.php %TEMP%\%NAME%\
xcopy /Q /I ..\%NAME%\*.ini %TEMP%\%NAME%\
xcopy /Q /I ..\%NAME%\*.txt %TEMP%\%NAME%\
xcopy /Q /I *.xml %TEMP%\%NAME%\

echo Packing files...
if "%newVersion%" NEQ "" set ZIP=%ZIP%-%newVersion%
set ZIP=%ZIP%.zip

pushd %TEMP%
rem %RAR% a -afzip -r %ZIP% 
%RAR% a -tzip -r -x@..\build-exclude.txt %ZIP% 

popd
move %TEMP%\%ZIP% .

echo Cleaning...
rmdir /S /Q %TEMP%

echo Done.
pause
goto :EOF


:UPDATE_VERSION
rem Parse out old version: should be in form major.minor.build
set oldVersion=
for /F "tokens=3 delims=<> " %%A in ('findstr /c:%2 %1') do (
    set oldVersion=%%A
)
rem Increase old version and write it to file
if "%oldVersion%" NEQ "" call :INCREASE_VERSION %1 %oldVersion%
goto :EOF


:INCREASE_VERSION
set major=
set minor=
set build=
for /F "tokens=1-8 delims=.-" %%A in ("%2" ) do (
    set major=%%A
    set minor=%%B
    set /A build=%%C+1
)
set newVersion=%major%.%minor%.%build%
if "%newVersion%" EQU ".." goto :EOF

rem Multiple occurence support: only the wished one should be changed
set entryNumber=%3
if "%entryNumber%" EQU "" set entryNumber=1

rem NOTE: This eliminates all the blank lines!
echo Version update %2 to %newVersion% in %1
for /F "tokens=* delims=" %%A in (%1) do (
    set str=%%A
    set newStr=!str:%2=%newVersion%!
    if "!newStr!" NEQ "!str!" set /A entryNumber=!entryNumber!-1
    if "!entryNumber!" EQU "0" (
        set entryNumber=-1
        set str=!newStr!
    )
    echo !str!>> temp.tmp
)
move temp.tmp %1
goto :EOF


:EOF
