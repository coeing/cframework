xcopy "%1"\bin\libstlport.5.1.dll 		"%1"\build\"%2"\ 			/d
xcopy "%1"\bin\libtiff3.dll 			"%1"\build\"%2"\ 			/d
xcopy "%1"\bin\jpeg62.dll 				"%1"\build\"%2"\ 			/d
xcopy "%1"\bin\zlib1.dll 				"%1"\build\"%2"\ 			/d
xcopy "%1"\lib\xerces-c_2_8.dll 		"%1"\build\"%2"\ 			/d
xcopy "%1"\tests\"%2"\data 				"%1"\build\"%2"\data\ 		/d /s /y
xcopy "%1"\data\common 					"%1"\build\"%2"\common\ 	/d /s /y
