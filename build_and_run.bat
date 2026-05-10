@echo off
echo ============================================
echo  MediCore - Build and Run (GUI)
echo ============================================
g++ main.cpp src/Person.cpp src/Patient.cpp src/Doctor.cpp src/Admin.cpp src/Appointment.cpp src/Bill.cpp src/Prescription.cpp src/FileHandler.cpp src/Validator.cpp src/HospitalException.cpp src/UIManager.cpp src/SceneManager.cpp src/SplashScreen.cpp src/LoginScreen.cpp src/PatientDashboard.cpp src/DoctorDashboard.cpp src/AdminDashboard.cpp src/Button.cpp src/InputBox.cpp src/Label.cpp src/Popup.cpp src/Dropdown.cpp -I "C:\SFML\include" -L "C:\SFML\lib" -o hospital_gui.exe -lsfml-graphics -lsfml-window -lsfml-system
if %errorlevel%==0 (
    echo Compilation Successful! Launching GUI...
    hospital_gui.exe
) else (
    echo Compilation FAILED. Errors shown above.
    pause
)
