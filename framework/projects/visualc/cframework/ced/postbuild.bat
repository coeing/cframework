xcopy "%2"\ext\freetype2\bin\freetype6.dll		"%1"\build /d /i /y
xcopy "%2"\ext\libtiff\bin\libtiff3.dll			"%1"\build /d /i /y
xcopy "%2"\ext\glpng\bin\zlib1.dll				"%1"\build /d /i /y
xcopy "%2"\ext\glpng\bin\jpeg62.dll				"%1"\build /d /i /y
xcopy "%2"\data 								"%1"\build\ /d /s /y
exit