@echo off
echo ========================================================
echo  Preparing MediCore Final Project for Submission...
echo ========================================================
echo.

echo 1. Cleaning up temporary object files (*.o)...
del /Q *.o

echo.
echo 2. Creating project zip file...
powershell -Command "Compress-Archive -Path '..\MediCore' -DestinationPath '..\MediCore_Final_Project.zip' -Force"

if %errorlevel%==0 (
    echo.
    echo ========================================================
    echo  SUCCESS!
    echo  Your submission file has been created at:
    echo  d:\ISRA Project\MediCore_Final_Project.zip
    echo ========================================================
    echo This zip file contains the source code, data files,
    echo required DLLs, and the hospital_gui.exe executable.
) else (
    echo.
    echo ========================================================
    echo  FAILED to create zip file.
    echo ========================================================
)

pause
