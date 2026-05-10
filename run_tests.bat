@echo off
echo ============================================
echo  MediCore - Backend Test Suite (No SFML)
echo ============================================
echo Compiling tests...
g++ gui_tests.cpp src/Person.cpp src/Patient.cpp src/Doctor.cpp src/Admin.cpp src/Appointment.cpp src/Bill.cpp src/Prescription.cpp src/FileHandler.cpp src/Validator.cpp src/HospitalException.cpp -o run_tests.exe
if %errorlevel%==0 (
    echo Running all tests...
    echo.
    run_tests.exe
) else (
    echo Test compilation FAILED.
    pause
)
pause
