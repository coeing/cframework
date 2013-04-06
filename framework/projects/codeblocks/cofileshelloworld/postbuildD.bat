md "%1"\build
xcopy "%2"\ext\stlport\bin\libstlport.5.1.dll   "%1"\build /i /y
md "%1"\build\data
xcopy "%1"\data                                 "%1"\build\data /d /y
xcopy bin\Debug\cofileshelloworldD.exe          "%1"\build /d /y
