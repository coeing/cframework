@echo off

set wORK_DIR=%cd%
set DIR=.\tmp
set FILE=cframework_lib.zip

echo Make directories...
echo -------------------

IF EXIST %DIR% rd /s /q %DIR%
md %DIR% > NUL
md %DIR%\include > NUL

echo Export libraries...
echo -------------------

set LIBS=cfoundation csystem cgui cguicontrols cctrl

svn export ./data %DIR%/data > NUL
svn export ./bin %DIR%/bin > NUL
xcopy .\lib	%DIR%\lib\ /d /s /y > NUL

for %%L in (%LIBS%) do (
  svn export --force ./core/%%L/include %DIR%/include > NUL
)

echo Exporting external files...
echo ---------------------------

set EXT_FILES=./ext/freetype2/bin ./ext/libtiff/bin ./ext/glpng/bin

for %%F in (%EXT_FILES%) do (
  md "%DIR%/%%F"
  svn export --force %%F %DIR%/%%F > NUL
)

echo Archive files to %WORK_DIR%\%FILE%...
echo -------------------------------------

cd %DIR%
7z a -mx5 -r -tzip %WORK_DIR%/%FILE% * > NUL

echo Removing tmp directory...
echo -------------------------

cd %WORK_DIR%
rd /s /q %DIR%

echo All done
echo --------

