#include "../include/DoctorDashboard.h"
#include "../include/LoginScreen.h"
#include "../include/UIManager.h"

static const sf::Color D_SIDEBAR (30, 39, 46);
static const sf::Color D_HDR     (39,174, 96);
static const sf::Color D_IDLE    (44, 62, 80);
static const sf::Color D_HOVER   (52,152,219);
static const sf::Color D_ACTIVE  (41,128,185);
static const sf::Color D_CONTENT (245,248,250);

// ─────────────────────────────────────────────────────────────
DoctorDashboard::DoctorDashboard()
    : currentUser(nullptr), welcomeLabel(nullptr), roleLabel(nullptr),
      btnViewSchedule(nullptr), btnMarkComplete(nullptr), btnMarkNoShow(nullptr),
      btnWritePrescription(nullptr), btnPatientHistory(nullptr), btnLogout(nullptr),
      contentTitle(nullptr),
      dropdownAppts(nullptr), dropdownApptLabel(nullptr), confirmApptBtn(nullptr),
      dropdownPatients(nullptr), dropdownPatientLabel(nullptr), confirmPatientBtn(nullptr),
      inputField(nullptr), inputField2(nullptr), inputLabel(nullptr), inputLabel2(nullptr),
      submitBtn(nullptr), cancelInputBtn(nullptr), popup(nullptr), activeAction(0)
{
    tempApptID[0]=tempPatientID[0]=tempMeds[0]=tempNotes[0]=tempPrescID[0]='\0';
}

DoctorDashboard::~DoctorDashboard() {
    delete welcomeLabel;  delete roleLabel;
    delete btnViewSchedule; delete btnMarkComplete; delete btnMarkNoShow;
    delete btnWritePrescription; delete btnPatientHistory; delete btnLogout;
    delete contentTitle;
    delete dropdownAppts; delete dropdownApptLabel; delete confirmApptBtn;
    delete dropdownPatients; delete dropdownPatientLabel; delete confirmPatientBtn;
    delete inputField; delete inputField2;
    delete inputLabel; delete inputLabel2;
    delete submitBtn;  delete cancelInputBtn;
    delete popup;
}

// ─────────────────────────────────────────────────────────────
void DoctorDashboard::init() {
    UIManager* ui  = UIManager::getInstance();
    sf::Font&  font = ui->getFont();
    sf::Vector2u ws = ui->getWindow()->getSize();

    currentUser = ui->getDoctors()->findByID(ui->getLoggedInUserID());

    char wel[150]; strCopy(wel,"Welcome, Dr. "); strCat(wel, currentUser->getName());
    welcomeLabel = new Label(260, 18, wel, font, 24, sf::Color::White);
    roleLabel    = new Label(260, 48, "[ DOCTOR PORTAL ]", font, 14, sf::Color(169,223,191));

    float sx=10, sw=220, sh=42, sy=120, gap=8;
    btnViewSchedule      = new Button(sx,sy,sw,sh,"Today's Schedule",   font,D_IDLE,D_HOVER,D_ACTIVE); sy+=sh+gap;
    btnMarkComplete      = new Button(sx,sy,sw,sh,"Mark Complete",       font,D_IDLE,D_HOVER,D_ACTIVE); sy+=sh+gap;
    btnMarkNoShow        = new Button(sx,sy,sw,sh,"Mark No-Show",        font,D_IDLE,D_HOVER,D_ACTIVE); sy+=sh+gap;
    btnWritePrescription = new Button(sx,sy,sw,sh,"Write Prescription",  font,D_IDLE,D_HOVER,D_ACTIVE); sy+=sh+gap;
    btnPatientHistory    = new Button(sx,sy,sw,sh,"Patient History",     font,D_IDLE,D_HOVER,D_ACTIVE);
    btnLogout = new Button(sx,(float)ws.y-60,sw,sh,"Logout",font,sf::Color(192,57,43),sf::Color(231,76,60),sf::Color(169,50,38));

    contentPanel.setPosition(242,80);
    contentPanel.setSize(sf::Vector2f((float)ws.x-242,(float)ws.y-80));
    contentPanel.setFillColor(D_CONTENT);

    contentTitle = new Label(260,95,"Select an action from the sidebar.",font,20,sf::Color(44,62,80));

    // ── Appointment dropdown (Mark Complete / No-Show / Prescription)
    dropdownApptLabel = new Label(260,145,"Select Appointment:",font,16,sf::Color(44,62,80));
    dropdownAppts     = new Dropdown(260,168,500,42,"-- Select Appointment --",font);
    confirmApptBtn    = new Button(260,400,220,42,"Confirm Selection",font,D_HOVER,D_ACTIVE,D_ACTIVE);

    // ── Patient dropdown (Patient History)
    dropdownPatientLabel = new Label(260,145,"Select Patient:",font,16,sf::Color(44,62,80));
    dropdownPatients     = new Dropdown(260,168,460,42,"-- Select Patient --",font);
    confirmPatientBtn    = new Button(260,400,220,42,"View History",font,D_HOVER,D_ACTIVE,D_ACTIVE);

    // ── Prescription text inputs (appear after appointment selected)
    inputLabel  = new Label(260,330,"Enter Medicines (comma separated):",font,16,sf::Color(44,62,80));
    inputField  = new InputBox(260,352,480,42,"e.g. Paracetamol 500mg, Amoxicillin 250mg",font,false);
    inputLabel2 = new Label(260,410,"Additional Notes / Instructions:",font,16,sf::Color(44,62,80));
    inputField2 = new InputBox(260,432,480,42,"e.g. Take after meals, 3 times daily",font,false);
    submitBtn      = new Button(260,488,200,42,"Save Prescription",font,sf::Color(39,174,96),sf::Color(46,204,113),sf::Color(34,153,84));
    cancelInputBtn = new Button(472,488,120,42,"Cancel",font,sf::Color(127,140,141),sf::Color(99,110,114),sf::Color(86,101,115));

    popup = new Popup(font, ws);
}

// ─────────────────────────────────────────────────────────────
void DoctorDashboard::loadMyPendingAppointments() {
    UIManager* ui = UIManager::getInstance();
    dropdownAppts->clearItems();
    dropdownAppts->clearSelection();
    for (int i=0;i<ui->getAppointments()->size();i++) {
        Appointment* a = ui->getAppointments()->at(i);
        if (strCompare(a->getDoctorID(), currentUser->getID())==0 &&
            strCompare(a->getStatus(),"Pending")==0) {
            char display[150];
            strCopy(display, a->getID()); strCat(display," | Patient: ");
            strCat(display, a->getPatientID()); strCat(display," | ");
            strCat(display, a->getDate()); strCat(display," ");
            strCat(display, a->getTimeSlot());
            dropdownAppts->addItem(display, a->getID());
        }
    }
}

void DoctorDashboard::loadMyCompletedAppointments() {
    UIManager* ui = UIManager::getInstance();
    dropdownAppts->clearItems();
    dropdownAppts->clearSelection();
    for (int i=0;i<ui->getAppointments()->size();i++) {
        Appointment* a = ui->getAppointments()->at(i);
        if (strCompare(a->getDoctorID(), currentUser->getID())==0 &&
            strCompare(a->getStatus(),"Completed")==0) {
            char display[150];
            strCopy(display, a->getID()); strCat(display," | Patient: ");
            strCat(display, a->getPatientID()); strCat(display," | ");
            strCat(display, a->getDate()); strCat(display," ");
            strCat(display, a->getTimeSlot());
            dropdownAppts->addItem(display, a->getID());
        }
    }
}

void DoctorDashboard::loadMyPatients() {
    UIManager* ui = UIManager::getInstance();
    dropdownPatients->clearItems();
    dropdownPatients->clearSelection();
    // Find unique patient IDs that have appointments with this doctor
    char seenIDs[50][30]; int seenCount=0;
    for (int i=0;i<ui->getAppointments()->size();i++) {
        Appointment* a = ui->getAppointments()->at(i);
        if (strCompare(a->getDoctorID(), currentUser->getID())!=0) continue;
        const char* pid = a->getPatientID();
        // check duplicate
        bool dup=false;
        for (int k=0;k<seenCount;k++) if (strCompare(seenIDs[k],pid)==0) { dup=true; break; }
        if (dup) continue;
        strCopy(seenIDs[seenCount++], pid);
        // build display
        Patient* p = ui->getPatients()->findByID(pid);
        char display[150]; strCopy(display, pid);
        if (p) { strCat(display," - "); strCat(display, p->getName()); strCat(display," | Age: "); char astr[6]; intToChar(astr,p->getAge()); strCat(display,astr); }
        dropdownPatients->addItem(display, pid);
    }
}

// ─────────────────────────────────────────────────────────────
void DoctorDashboard::clearAction() {
    activeAction=0;
    dropdownAppts->clearSelection();
    dropdownPatients->clearSelection();
    inputField->clear(); inputField2->clear();
}

// ─────────────────────────────────────────────────────────────
void DoctorDashboard::handleEvent(sf::Event& event, sf::RenderWindow& window) {
    if (popup->isVisible()) { popup->handleEvent(event,window); return; }

    btnViewSchedule->handleEvent(event,window);
    btnMarkComplete->handleEvent(event,window);
    btnMarkNoShow->handleEvent(event,window);
    btnWritePrescription->handleEvent(event,window);
    btnPatientHistory->handleEvent(event,window);
    btnLogout->handleEvent(event,window);

    if (activeAction==10 || activeAction==20 || activeAction==30) {
        dropdownAppts->handleEvent(event,window);
        confirmApptBtn->handleEvent(event,window);
    }
    if (activeAction==31) {
        inputField->handleEvent(event,window);
        inputField2->handleEvent(event,window);
        submitBtn->handleEvent(event,window);
    }
    if (activeAction==40) {
        dropdownPatients->handleEvent(event,window);
        confirmPatientBtn->handleEvent(event,window);
    }
    if (activeAction!=0) cancelInputBtn->handleEvent(event,window);
}

// ─────────────────────────────────────────────────────────────
void DoctorDashboard::update(float dt) {
    if (popup->isVisible()) { popup->update(dt); return; }

    UIManager* ui = UIManager::getInstance();
    btnViewSchedule->update(dt);    btnMarkComplete->update(dt);
    btnMarkNoShow->update(dt);      btnWritePrescription->update(dt);
    btnPatientHistory->update(dt);  btnLogout->update(dt);

    if (activeAction==10 || activeAction==20 || activeAction==30) {
        dropdownAppts->update(dt);
        confirmApptBtn->update(dt);
    }
    if (activeAction==31) {
        inputField->update(dt); inputField2->update(dt); submitBtn->update(dt);
    }
    if (activeAction==40) {
        dropdownPatients->update(dt);
        confirmPatientBtn->update(dt);
    }
    if (activeAction!=0) {
        cancelInputBtn->update(dt);
        if (cancelInputBtn->isPressed()) { clearAction(); contentTitle->setText("Cancelled."); return; }
    }

    // 
    // View Today's Schedule
    // 
    if (btnViewSchedule->isPressed()) {
        clearAction();
        char info[800]; strCopy(info,"");
        int cnt=0;
        for (int i=0;i<ui->getAppointments()->size();i++) {
            Appointment* a = ui->getAppointments()->at(i);
            if (strCompare(a->getDoctorID(), currentUser->getID())==0) {
                // look up patient name
                Patient* p = ui->getPatients()->findByID(a->getPatientID());
                strCat(info, a->getID()); strCat(info," | ");
                if (p) { strCat(info, p->getName()); } else { strCat(info, a->getPatientID()); }
                strCat(info," | "); strCat(info, a->getDate()); strCat(info," ");
                strCat(info, a->getTimeSlot()); strCat(info," | "); strCat(info, a->getStatus()); strCat(info,"\n");
                cnt++;
            }
        }
        contentTitle->setText("All My Appointments:");
        if (cnt==0) popup->show("Schedule","No appointments found.");
        else        popup->show("All Appointments", info);
    }

    
    // Mark Complete — dropdown of Pending appointments
    
    if (btnMarkComplete->isPressed()) {
        loadMyPendingAppointments();
        if (dropdownAppts->getSelectedIndex()==-1 && dropdownAppts->itemCount()==0) {
            popup->show("No Appointments","No pending appointments available."); return;
        }
        contentTitle->setText("Mark Appointment as COMPLETED");
        activeAction=10;
    }
    if (activeAction==10 && confirmApptBtn->isPressed()) {
        if (!dropdownAppts->hasSelection()) { popup->show("Error","Please select an appointment."); return; }
        strCopy(tempApptID, dropdownAppts->getSelectedValue());
        Appointment* a = ui->getAppointments()->findByID(tempApptID);
        if (!a) { popup->show("Error","Appointment not found."); return; }
        a->setStatus("Completed");
        ui->getFileHandler()->rewriteAppointments(ui->getAppointments());
        popup->show("Done!","Appointment marked as COMPLETED.");
        clearAction(); contentTitle->setText("Status updated.");
    }

    // Mark No-Show — dropdown of Pending appointments
   
    if (btnMarkNoShow->isPressed()) {
        loadMyPendingAppointments();
        if (dropdownAppts->itemCount()==0) { popup->show("No Appointments","No pending appointments available."); return; }
        contentTitle->setText("Mark Appointment as NO-SHOW");
        activeAction=20;
    }
    if (activeAction==20 && confirmApptBtn->isPressed()) {
        if (!dropdownAppts->hasSelection()) { popup->show("Error","Please select an appointment."); return; }
        strCopy(tempApptID, dropdownAppts->getSelectedValue());
        Appointment* a = ui->getAppointments()->findByID(tempApptID);
        if (!a) { popup->show("Error","Appointment not found."); return; }
        a->setStatus("No-Show");
        ui->getFileHandler()->rewriteAppointments(ui->getAppointments());
        popup->show("Done!","Appointment marked as NO-SHOW.");
        clearAction(); contentTitle->setText("Status updated.");
    }

    // Write Prescription — Step 1: pick completed appt
   
    if (btnWritePrescription->isPressed()) {
        loadMyCompletedAppointments();
        if (dropdownAppts->itemCount()==0) { popup->show("No Completed Appointments","Mark an appointment as Completed first."); return; }
        contentTitle->setText("Write Prescription - Select Completed Appointment");
        activeAction=30;
    }
    if (activeAction==30 && confirmApptBtn->isPressed()) {
        if (!dropdownAppts->hasSelection()) { popup->show("Error","Please select an appointment."); return; }
        strCopy(tempApptID, dropdownAppts->getSelectedValue());
        Appointment* a = ui->getAppointments()->findByID(tempApptID);
        if (!a) { popup->show("Error","Appointment not found."); return; }
        strCopy(tempPatientID, a->getPatientID());
        inputField->clear(); inputField2->clear();
        contentTitle->setText("Write Prescription - Enter Medicines & Notes");
        activeAction=31;
    }
    // Step 2: enter medicines + notes
    if (activeAction==31 && submitBtn->isPressed()) {
        strCopy(tempMeds,  inputField->getText());
        strCopy(tempNotes, inputField2->getText());
        if (strLen(tempMeds)==0) { popup->show("Error","Medicines cannot be empty."); return; }
        Appointment* a = ui->getAppointments()->findByID(tempApptID);
        char dateStr[15]; strCopy(dateStr, a ? a->getDate() : "");
        generateNextID(tempPrescID,"PR", ui->getPrescriptions()->size()+1);
        Prescription presc(tempPrescID, tempApptID, tempPatientID, currentUser->getID(), dateStr, tempMeds, tempNotes);
        ui->getPrescriptions()->add(presc);
        char csv[1024]; presc.toCSV(csv);
        ui->getFileHandler()->appendRecord("prescriptions.txt", csv);
        char ok[80]; strCopy(ok,"Prescription ID: "); strCat(ok,tempPrescID);
        popup->show("Prescription Saved!",ok);
        clearAction(); contentTitle->setText("Prescription saved.");
    }


    // Patient History — dropdown of patients seen by this doctor
   
    if (btnPatientHistory->isPressed()) {
        loadMyPatients();
        if (dropdownPatients->itemCount()==0) { popup->show("No Patients","You have not seen any patients yet."); return; }
        contentTitle->setText("Patient History - Select a Patient");
        activeAction=40;
    }
    if (activeAction==40 && confirmPatientBtn->isPressed()) {
        if (!dropdownPatients->hasSelection()) { popup->show("Error","Please select a patient."); return; }
        strCopy(tempPatientID, dropdownPatients->getSelectedValue());
        char info[800]; strCopy(info,"");
        int cnt=0;
        for (int i=0;i<ui->getPrescriptions()->size();i++) {
            Prescription* p = ui->getPrescriptions()->at(i);
            if (strCompare(p->getPatientID(), tempPatientID)==0) {
                strCat(info, p->getID()); strCat(info," | "); strCat(info, p->getDate());
                strCat(info,"\nMeds: "); strCat(info, p->getMedicines()); strCat(info,"\n\n");
                cnt++;
            }
        }
        if (cnt==0) popup->show("History","No prescriptions found for this patient.");
        else {
            Patient* p = ui->getPatients()->findByID(tempPatientID);
            char title[80]; strCopy(title,"History for: ");
            if (p) strCat(title, p->getName()); else strCat(title, tempPatientID);
            popup->show(title, info);
        }
    }

    // Logout
    
    if (btnLogout->isPressed()) {
        ui->logout();
        ui->getSceneManager()->changeScene(new LoginScreen());
    }
}


void DoctorDashboard::draw(sf::RenderWindow& window) {
    sf::RectangleShape sidebar(sf::Vector2f(240,(float)window.getSize().y));
    sidebar.setFillColor(D_SIDEBAR); window.draw(sidebar);

    sf::RectangleShape header(sf::Vector2f((float)window.getSize().x,80));
    header.setFillColor(D_HDR); window.draw(header);

    welcomeLabel->draw(window);
    roleLabel->draw(window);
    window.draw(contentPanel);
    contentTitle->draw(window);

    btnViewSchedule->draw(window);    btnMarkComplete->draw(window);
    btnMarkNoShow->draw(window);      btnWritePrescription->draw(window);
    btnPatientHistory->draw(window);  btnLogout->draw(window);

    // Appointment dropdown (Mark Complete / No-Show / Prescription step 1)
    if (activeAction==10 || activeAction==20 || activeAction==30) {
        dropdownApptLabel->draw(window);
        dropdownAppts->draw(window);
        confirmApptBtn->draw(window);
        cancelInputBtn->draw(window);
    }
    // Prescription step 2 (medicines + notes)
    if (activeAction==31) {
        inputLabel->draw(window);  inputField->draw(window);
        inputLabel2->draw(window); inputField2->draw(window);
        submitBtn->draw(window);   cancelInputBtn->draw(window);
    }
    // Patient history dropdown
    if (activeAction==40) {
        dropdownPatientLabel->draw(window);
        dropdownPatients->draw(window);
        confirmPatientBtn->draw(window);
        cancelInputBtn->draw(window);
    }

    popup->draw(window);
}
