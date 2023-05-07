(gecho szhcSZHCGaussCheng
gecho szhcSZHCGaussCheng) | bcrypt %1.tar.bfe
tar.exe -xf %1.tar
set cc=%cd%
cd %1
tar.exe -xf EC_Motion.tar
tar -xzvf HCQkHost.bin.tar
rd /s /q %cc%\..\.tmp
mkdir %cc%\..\.tmp\
copy HCQkHost.bin %cc%\..\.tmp\
cd ..
rd /s /q %1
del /f /s /q %1.tar