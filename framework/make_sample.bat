@echo off

set WORK_DIR=%cd%
set DIR=.\tmp
set SAMPLE=%1%
set FILE=cframework_sample_%SAMPLE%.zip

echo Exporting sample '%SAMPLE%'
echo ----------------------------

echo Make directories...
echo -------------------

IF EXIST %DIR% rd /s /q %DIR%
md %DIR%
md %DIR%\samples
md %DIR%\projects\visualc\cframework

echo Export source and project files...
echo ----------------------------------

:export
if "%1" == "" goto export_end

set SAMPLE_SUB=%1
set README_FILE=readme_%SAMPLE_SUB%.txt

svn export --force ./samples/%SAMPLE_SUB% %DIR%/samples/%SAMPLE_SUB% > NUL
svn export --force ./projects/visualc/cframework/%SAMPLE_SUB% %DIR%/projects/visualc/cframework/%SAMPLE_SUB% > NUL
svn export --force ./projects/visualc/cframework/%SAMPLE_SUB%.sln %DIR%/projects/visualc/cframework/%SAMPLE_SUB%.sln > NUL 2>&1
svn export --force ./samples/%SAMPLE_SUB%/readme.txt %DIR%/%README_FILE% > NUL 2>&1

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
