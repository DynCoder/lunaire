cd scripts

echo building...
call build_debug.bat

cd ../bin

echo running...
echo.

lunaire.exe

echo.
cd ..

@pause