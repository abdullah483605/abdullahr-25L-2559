#ifndef DOCTORDASHBOARD_H
#define DOCTORDASHBOARD_H

#include "SceneManager.h"
#include "Button.h"
#include "InputBox.h"
#include "Label.h"
#include "Popup.h"
#include "Dropdown.h"
#include "Doctor.h"

class DoctorDashboard : public Scene {
private:
    Doctor* currentUser;

    Label* welcomeLabel;
    Label* roleLabel;

    // All 5 Doctor functions
    Button* btnViewSchedule;
    Button* btnMarkComplete;
    Button* btnMarkNoShow;
    Button* btnWritePrescription;
    Button* btnPatientHistory;
    Button* btnLogout;

    sf::RectangleShape contentPanel;
    Label* contentTitle;

    // Dropdown for appointment selection (Mark Complete / No-Show / Prescription)
    Dropdown* dropdownAppts;
    Label*    dropdownApptLabel;
    Button*   confirmApptBtn;

    // Dropdown for patient history (patients seen by this doctor)
    Dropdown* dropdownPatients;
    Label*    dropdownPatientLabel;
    Button*   confirmPatientBtn;

    // Text inputs for prescription medicines + notes
    InputBox* inputField;   // medicines
    InputBox* inputField2;  // notes
    Label*    inputLabel;
    Label*    inputLabel2;
    Button*   submitBtn;
    Button*   cancelInputBtn;

    Popup* popup;

    int  activeAction;
    char tempApptID[20];
    char tempPatientID[30];
    char tempMeds[300];
    char tempNotes[200];
    char tempPrescID[20];

    // Helpers
    void loadMyPendingAppointments();       // fill dropdownAppts with Pending
    void loadMyCompletedAppointments();     // fill dropdownAppts with Completed
    void loadMyPatients();                  // fill dropdownPatients with unique patients

    void clearAction();

public:
    DoctorDashboard();
    ~DoctorDashboard() override;

    void init() override;
    void handleEvent(sf::Event& event, sf::RenderWindow& window) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
};

#endif
