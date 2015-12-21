@echo off

call echo Clean previous build if any . . .
call bldmake clean
call bldmake bldfiles
call abld clean gcce
call abld reallyclean gcce
call echo Start build for GCCE . . .
call abld build gcce urel
call echo Start create SISX . . .
call cd ..\sis
call BuildSIS.bat
call cd ..\group
call pause