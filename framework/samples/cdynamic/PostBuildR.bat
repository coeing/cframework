xcopy ..\xerces\lib\xerces-c_2_7.dll			..\..\build\ced	    	/D /I /Y
xcopy release\ced.exe	    					..\..\build\ced	    	/D /I /Y
xcopy ..\..\data\common    						..\..\build\ced\common 	/S /E /K /D /I /Y
xcopy ..\..\data\ced    						..\..\build\ced\data   	/S /E /K /D /I /Y