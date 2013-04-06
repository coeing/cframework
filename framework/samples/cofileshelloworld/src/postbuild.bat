md "%1"\build
xcopy "%2"\ext\stlport\bin\libstlport.5.1.dll "%1"\build /i /y
xcopy "%CWD%"\cofileshelloworld.exe "%1"\build /i /y
