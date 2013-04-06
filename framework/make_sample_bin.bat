@echo off

set WORK_DIR=%cd%
set DIR=.\tmp
set SAMPLE=%1%
set FILE=cframework_sample_%SAMPLE%_bin.zip

echo Exporting sample binaries '%SAMPLE%'
echo ------------------------------------

echo Make directories...
echo -------------------

IF EXIST %DIR% rd /s /q %DIR%
md %DIR%
md %DIR%\samples

echo Export binary files...
echo ----------------------

:export
if "%1" == "" goto export_end

set SAMPLE_SUB=%1
set README_FILE=readme_%SAMPLE_SUB%.txt
set SAMPLE_BUILD=.\samples\%SAMPLE_SUB%\build
set SAMPLE_BUILD_TARGET=%DIR%\samples\%SAMPLE_SUB%\build

md %DIR%\samples\%SAMPLE_SUB%
svn export --force %SAMPLE_BUILD% %SAMPLE_BUILD_TARGET% > NUL
svn export --force ./samples/%SAMPLE_SUB%/readme.txt %DIR%/%README_FILE% > NUL 2>&1

echo Copy exe...
echo -----------
xcopy %SAMPLE_BUILD%\%SAMPLE%.exe	%SAMPLE_BUILD_TARGET% /y
xcopy %SAMPLE_BUILD%\*.dll		%SAMPLE_BUILD_TARGET% /y
svn export --force ./data %SAMPLE_BUILD_TARGET% > NUL

shift
goto export
:export_end

echo Archive files to %WORK_DIR%\%FILE%...
echo -------------------------------------

cd %DIR%
7z a -mx9 -r -tzip %WORK_DIR%/%FILE% * > NUL

echo Removing tmp directory...
echo -------------------------

cd %WORK_DIR%
rd /s /q %DIR%
