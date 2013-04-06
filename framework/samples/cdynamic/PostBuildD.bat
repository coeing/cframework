xcopy ..\xerces\lib\xerces-c_2_7D.dll			..\..\build\cdynamic	    	/D /I /Y
xcopy debug\cdynamicD.exe	    					..\..\build\cdynamic	    	/D /I /Y
xcopy ..\..\data\common    						..\..\build\cdynamic\common 	/S /E /K /D /I /Y
xcopy ..\..\data\cdynamic    						..\..\build\cdynamic\data   	/S /E /K /D /I /Y