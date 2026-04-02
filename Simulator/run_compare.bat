@echo off
echo ===========================================
echo   OS Algorithm Networking Simulator Test   
echo ===========================================
echo.
echo NOTE: Ensure you ran build.bat first!
echo.

if exist server_log.txt del server_log.txt

echo -------------------------------------------
echo [1/3] Testing FCFS algorithm
echo -------------------------------------------
start "Server FCFS" cmd /c "server.exe FCFS"
timeout /t 2 /nobreak >nul
client.exe
timeout /t 5 /nobreak >nul

echo.
echo -------------------------------------------
echo [2/3] Testing ROUND_ROBIN algorithm
echo -------------------------------------------
start "Server ROUND_ROBIN" cmd /c "server.exe RR"
timeout /t 2 /nobreak >nul
client.exe
timeout /t 5 /nobreak >nul

echo.
echo -------------------------------------------
echo [3/3] Testing PRIORITY algorithm
echo -------------------------------------------
start "Server PRIORITY" cmd /c "server.exe PRIORITY"
timeout /t 2 /nobreak >nul
client.exe
timeout /t 5 /nobreak >nul

echo.
echo All tests complete! 
echo Check server_log.txt for detailed logs and metrics.
pause
