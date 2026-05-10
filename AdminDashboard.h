#ifndef ADMINDASHBOARD_H
#define ADMINDASHBOARD_H

#include "SceneManager.h"
#include "Button.h"
#include "InputBox.h"
#include "Label.h"
#include "Popup.h"
#include "Admin.h"

class AdminDashboard : public Scene {
private:
    Admin* currentUser;

    Label* welcomeLabel;
    Label* roleLabel;

    // All 9 Admin functions
    Button* btnViewPatients;
    Button* btnViewDoctors;
    Button* btnViewAppts;
    Button* btnViewBills;
    Button* btnAddDoctor;
    Button* btnRemoveDoctor;
    Button* btnAddPatient;
    Button* btnDischarge;
    Button* btnSecurityLog;
    Button* btnReport;
    Button* btnLogout;

    sf::RectangleShape contentPanel;
    Label* contentTitle;

    InputBox* inputFields[6];
    Label*    inputLabels[6];
    int       numFields;

    Button*   submitBtn;
    Button*   cancelInputBtn;

    Popup* popup;

    int  activeAction;
    char tempDoctorID[30];
    char tempPatientID[30];

    void setMultiAction(int action, const char** labels, int count, const char* btnLbl);
    void clearAction();

public:
    AdminDashboard();
    ~AdminDashboard() override;

    void init() override;
    void handleEvent(sf::Event& event, sf::RenderWindow& window) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
};

#endif
