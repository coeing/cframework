@echo off

echo Make libs
echo *****************************

call make_lib.bat

echo Make sample "cctrlhelloworld"
echo *****************************

call make_sample_cctrlhelloworld.bat

echo Make sample "cguihelloworld"
echo *****************************

call make_sample_cguihelloworld.bat

echo Make sample "cetris"
echo *****************************

call make_sample_cetris.bat

echo Make sample "cmadtv"
echo *****************************

call make_sample_cmadtv.bat

pause
