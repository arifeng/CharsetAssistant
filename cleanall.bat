rem
rem  清除临时文件
rem


set SourcePath=.
del /s/q %SourcePath%\*.obj 
del /s/q %SourcePath%\*.opt
del /s/q %SourcePath%\*.ncb
del /s/q %SourcePath%\*.aps
del /s/q %SourcePath%\*.tlh
del /s/q %SourcePath%\*.tli
del /s/q %SourcePath%\*.pch
del /s/q %SourcePath%\*.pdb
del /s/q %SourcePath%\*.res
del /s/q %SourcePath%\*.sbr
del /s/q %SourcePath%\*.idb
del /s/q %SourcePath%\*.bsc
del /s/q %SourcePath%\*.exp
del /s/q %SourcePath%\*.ilk

