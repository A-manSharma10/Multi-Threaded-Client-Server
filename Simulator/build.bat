@echo off
set GCC_PATH="X:\from c\mingw64\mingw64\bin\gcc.exe"
echo Building OS Networking Simulator using %GCC_PATH%...

echo Compiling Server...
%GCC_PATH% server.cpp scheduler.cpp logger.cpp -o server.exe -lws2_32 -lstdc++ -std=c++11
if %errorlevel% neq 0 exit /b %errorlevel%

echo Compiling Client...
%GCC_PATH% client.cpp logger.cpp -o client.exe -lws2_32 -lstdc++ -std=c++11
if %errorlevel% neq 0 exit /b %errorlevel%

echo Build Successful!
