#ifndef PATIENTDASHBOARD_H
#define PATIENTDASHBOARD_H

#include "SceneManager.h"
#include "Button.h"
#include "InputBox.h"
#include "Label.h"
#include "Popup.h"
#include "Dropdown.h"
#include "Patient.h"

class PatientDashboard : public Scene {
private:
    Patient* currentUser;

    // Header labels
    Label* welcomeLabel;
    Label* balanceLabel;
    Label* roleLabel;

    // Sidebar buttons - ALL Patient functions
    Button* btnViewAppts;
    Button* btnBookAppt;
    Button* btnCancelAppt;
    Button* btnMedRecords;
    Button* btnViewBills;
    Button* btnPayBill;
    Button* btnTopUp;
    Button* btnLogout;

    // Content area
    sf::RectangleShape contentPanel;
    Label* contentTitle;

    // Doctor dropdown (for booking)
    Dropdown* dropdownDoctors;
    Label*    dropdownLabel;
    Button*   confirmDocBtn;

    // Input field for actions needing text input
    InputBox* inputField;
    Label*    inputLabel;
    Button*   submitBtn;
    Button*   cancelInputBtn;

    // Popup for feedback
    Popup* popup;

    // State machine for multi-step actions
    int  activeAction;  // 0=none, 1=bookStep1(spec), 2=bookStep2(docID), ...
    char tempSpec[100];
    char tempDocID[30];
    char tempDate[15];
    char tempSlot[10];
    char tempApptID[20];
    char tempBillID[20];

    void refreshBalance();
    void drawSidebar(sf::RenderWindow& window);
    void drawHeader(sf::RenderWindow& window);
    void setAction(int action, const char* inputLbl, const char* submitLbl);
    void clearAction();

public:
    PatientDashboard();
    ~PatientDashboard() override;

    void init() override;
    void handleEvent(sf::Event& event, sf::RenderWindow& window) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
};

#endif
