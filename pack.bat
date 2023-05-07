
rmdir /q /s WinPanelOutput
set SOFT_VERSION=%1%
MKDIR WinPanelOutput\%SOFT_VERSION%
MKDIR WinPanelOutput\%SOFT_VERSION%\tool
@xcopy .\pack WinPanelOutput\%SOFT_VERSION% /s/e/i/y 
@xcopy .\tool WinPanelOutput\%SOFT_VERSION%\tool /s/e/i/y
@copy release\EC-Slave-Debug.exe WinPanelOutput\%SOFT_VERSION%\  