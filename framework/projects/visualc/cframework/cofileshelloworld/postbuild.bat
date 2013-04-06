IF NOT EXIST "%1"\build md "%1"\build
IF NOT EXIST "%1"\build\data md "%1"\build\data
xcopy "%1"\data                                 "%1"\build\data /d /y
