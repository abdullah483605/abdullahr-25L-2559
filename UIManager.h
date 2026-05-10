#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <SFML/Graphics.hpp>
#include "SceneManager.h"
#include "FileHandler.h"
#include "Storage.h"
#include "Patient.h"
#include "Doctor.h"
#include "Admin.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"
#include "Validator.h"

class UIManager {
private:
    static UIManager* instance;
    
    sf::RenderWindow* window;
    SceneManager* sceneManager;
    sf::Font mainFont;
    
    // Backend Logic Core
    FileHandler* fileHandler;
    Validator* validator;
    
    Storage<Patient> patients;
    Storage<Doctor> doctors;
    Storage<Admin> admins;
    Storage<Appointment> appointments;
    Storage<Bill> bills;
    Storage<Prescription> prescriptions;
    
    // Session tracking
    char* loggedInUserID;
    const char* userRole; // "Patient", "Doctor", "Admin"
    
    UIManager();
    ~UIManager();

public:
    static UIManager* getInstance();
    static void cleanup();

    void init(sf::RenderWindow* win);
    void run();
    
    // Accessors
    sf::RenderWindow* getWindow() { return window; }
    SceneManager* getSceneManager() { return sceneManager; }
    sf::Font& getFont() { return mainFont; }
    
    FileHandler* getFileHandler() { return fileHandler; }
    Validator* getValidator() { return validator; }
    
    Storage<Patient>* getPatients() { return &patients; }
    Storage<Doctor>* getDoctors() { return &doctors; }
    Storage<Admin>* getAdmins() { return &admins; }
    Storage<Appointment>* getAppointments() { return &appointments; }
    Storage<Bill>* getBills() { return &bills; }
    Storage<Prescription>* getPrescriptions() { return &prescriptions; }
    
    void login(const char* id, const char* role);
    void logout();
    const char* getLoggedInUserID() const { return loggedInUserID; }
    const char* getUserRole() const { return userRole; }
    
    void loadData();
};

#endif
