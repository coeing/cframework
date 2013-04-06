md "%1"\build
xcopy "%2"\ext\stlport\bin\libstlport.5.1.dll   "%1"\build /d /i /y
xcopy "%2"\ext\freetype2\bin\freetype6.dll      "%1"\build /d /i /y
xcopy "%2"\ext\glpng\bin\zlib1.dll              "%1"\build /d /i /y
xcopy "%2"\ext\glpng\bin\jpeg62.dll             "%1"\build /d /i /y
xcopy "%2"\ext\libtiff\bin\libtiff3.dll         "%1"\build /d /i /y

md "%1"\build\common
xcopy "%2"\core\cgui\data 						"%1"\build\ /d /s /y

md "%1"\build\data
xcopy "%1"\data                                 "%1"\build\data /d /s /y
xcopy bin\Debug\cguihelloworldD.exe             "%1"\build /d /y
