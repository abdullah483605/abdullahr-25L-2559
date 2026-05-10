s#include "../include/AdminDashboard.h"
#include "../include/LoginScreen.h"
#include "../include/UIManager.h"

static const sf::Color A_SIDEBAR(30,39,46);
static const sf::Color A_HDR    (142,68,173);
static const sf::Color A_IDLE   (44,62,80);
static const sf::Color A_HOVER  (52,152,219);
static const sf::Color A_ACTIVE (41,128,185);

AdminDashboard::AdminDashboard()
    : currentUser(nullptr), welcomeLabel(nullptr), roleLabel(nullptr),
      btnViewPatients(nullptr), btnViewDoctors(nullptr), btnViewAppts(nullptr),
      btnViewBills(nullptr), btnAddDoctor(nullptr), btnRemoveDoctor(nullptr),
      btnAddPatient(nullptr), btnDischarge(nullptr), btnSecurityLog(nullptr), btnReport(nullptr),
      btnLogout(nullptr), contentTitle(nullptr), submitBtn(nullptr),
      cancelInputBtn(nullptr), popup(nullptr), activeAction(0), numFields(0) {
    for (int i=0;i<6;i++) { inputFields[i]=nullptr; inputLabels[i]=nullptr; }
    tempDoctorID[0]=tempPatientID[0]='\0';
}

AdminDashboard::~AdminDashboard() {
    delete welcomeLabel; delete roleLabel;
    delete btnViewPatients; delete btnViewDoctors; delete btnViewAppts;
    delete btnViewBills; delete btnAddDoctor; delete btnRemoveDoctor; delete btnAddPatient;
    delete btnDischarge; delete btnSecurityLog; delete btnReport; delete btnLogout;
    delete contentTitle; delete submitBtn; delete cancelInputBtn; delete popup;
    for (int i=0;i<6;i++) { delete inputFields[i]; delete inputLabels[i]; }
}

void AdminDashboard::init() {
    UIManager* ui = UIManager::getInstance();
    sf::Font& font = ui->getFont();
    sf::Vector2u ws = ui->getWindow()->getSize();

    currentUser = ui->getAdmins()->findByID(ui->getLoggedInUserID());

    char wel[150]; strCopy(wel,"Welcome, Admin "); strCat(wel, currentUser->getName());
    welcomeLabel = new Label(260,18,wel,font,24,sf::Color::White);
    roleLabel    = new Label(260,48,"[ ADMIN PORTAL ]",font,14,sf::Color(210,180,222));

    float sx=10,sw=220,sh=38,sy=90,gap=6;
    btnViewPatients = new Button(sx,sy,sw,sh,"View All Patients",  font,A_IDLE,A_HOVER,A_ACTIVE); sy+=sh+gap;
    btnViewDoctors  = new Button(sx,sy,sw,sh,"View All Doctors",   font,A_IDLE,A_HOVER,A_ACTIVE); sy+=sh+gap;
    btnViewAppts    = new Button(sx,sy,sw,sh,"View Appointments",  font,A_IDLE,A_HOVER,A_ACTIVE); sy+=sh+gap;
    btnViewBills    = new Button(sx,sy,sw,sh,"Unpaid Bills",       font,A_IDLE,A_HOVER,A_ACTIVE); sy+=sh+gap;
    btnAddDoctor    = new Button(sx,sy,sw,sh,"Add Doctor",         font,A_IDLE,A_HOVER,A_ACTIVE); sy+=sh+gap;
    btnRemoveDoctor = new Button(sx,sy,sw,sh,"Remove Doctor",      font,A_IDLE,A_HOVER,A_ACTIVE); sy+=sh+gap;
    btnAddPatient   = new Button(sx,sy,sw,sh,"Register Patient",   font,A_IDLE,A_HOVER,A_ACTIVE); sy+=sh+gap;
    btnDischarge    = new Button(sx,sy,sw,sh,"Discharge Patient",  font,A_IDLE,A_HOVER,A_ACTIVE); sy+=sh+gap;
    btnSecurityLog  = new Button(sx,sy,sw,sh,"Security Log",       font,A_IDLE,A_HOVER,A_ACTIVE); sy+=sh+gap;
    btnReport       = new Button(sx,sy,sw,sh,"Daily Report",       font,A_IDLE,A_HOVER,A_ACTIVE);
    btnLogout       = new Button(sx,(float)ws.y-55,sw,sh,"Logout", font,sf::Color(192,57,43),sf::Color(231,76,60),sf::Color(169,50,38));

    contentPanel.setPosition(242,80);
    contentPanel.setSize(sf::Vector2f((float)ws.x-242,(float)ws.y-80));
    contentPanel.setFillColor(sf::Color(245,248,250));

    contentTitle = new Label(260,90,"Select an action from the sidebar.",font,20,sf::Color(44,62,80));

    // Create 6 input rows
    for (int i=0;i<6;i++) {
        inputLabels[i] = new Label(260, 140+(i*80), "", font, 16, sf::Color(44,62,80));
        inputFields[i] = new InputBox(260, 162+(i*80), 380, 38, "", font, false);
    }
    submitBtn      = new Button(260, 640, 180, 40, "Submit", font, A_HOVER, A_ACTIVE, A_ACTIVE);
    cancelInputBtn = new Button(450, 640, 120, 40, "Cancel", font,
                                sf::Color(127,140,141),sf::Color(99,110,114),sf::Color(86,101,115));

    popup = new Popup(font, ws);
}

void AdminDashboard::setMultiAction(int action, const char** labels, int count, const char* btnLbl) {
    activeAction = action;
    numFields = count;
    for (int i=0;i<6;i++) {
        if (i<count) { inputLabels[i]->setText(labels[i]); inputFields[i]->clear(); }
        else { inputLabels[i]->setText(""); }
    }
    submitBtn->setText(btnLbl);
}

void AdminDashboard::clearAction() {
    activeAction = 0; numFields = 0;
    for (int i=0;i<6;i++) { inputLabels[i]->setText(""); inputFields[i]->clear(); }
}

void AdminDashboard::handleEvent(sf::Event& event, sf::RenderWindow& window) {
    if (popup->isVisible()) { popup->handleEvent(event,window); return; }

    btnViewPatients->handleEvent(event,window); btnViewDoctors->handleEvent(event,window);
    btnViewAppts->handleEvent(event,window);    btnViewBills->handleEvent(event,window);
    btnAddDoctor->handleEvent(event,window);    btnRemoveDoctor->handleEvent(event,window);
    btnAddPatient->handleEvent(event,window);   btnDischarge->handleEvent(event,window);
    btnSecurityLog->handleEvent(event,window);
    btnReport->handleEvent(event,window);       btnLogout->handleEvent(event,window);

    if (activeAction!=0) {
        for (int i=0;i<numFields;i++) inputFields[i]->handleEvent(event,window);
        submitBtn->handleEvent(event,window);
        cancelInputBtn->handleEvent(event,window);
    }
}

void AdminDashboard::update(float dt) {
    if (popup->isVisible()) { popup->update(dt); return; }

    UIManager* ui = UIManager::getInstance();
    btnViewPatients->update(dt); btnViewDoctors->update(dt); btnViewAppts->update(dt);
    btnViewBills->update(dt);    btnAddDoctor->update(dt);   btnRemoveDoctor->update(dt);
    btnAddPatient->update(dt);   btnDischarge->update(dt);   btnSecurityLog->update(dt); 
    btnReport->update(dt);
    btnLogout->update(dt);

    if (activeAction!=0) {
        for (int i=0;i<numFields;i++) inputFields[i]->update(dt);
        submitBtn->update(dt); cancelInputBtn->update(dt);
        if (cancelInputBtn->isPressed()) { clearAction(); contentTitle->setText("Cancelled."); return; }
    }

    // ---- View All Patients ----
    if (btnViewPatients->isPressed()) {
        clearAction();
        char info[800]; strCopy(info,"");
        for (int i=0;i<ui->getPatients()->size();i++) {
            Patient* p = ui->getPatients()->at(i);
            strCat(info, p->getID()); strCat(info," | "); strCat(info, p->getName());
            strCat(info," | Age:"); char astr[6]; intToChar(astr,p->getAge()); strCat(info,astr);
            strCat(info," | "); strCat(info, p->getContact()); strCat(info,"\n");
        }
        contentTitle->setText("All Patients:");
        if (ui->getPatients()->size()==0) popup->show("Patients","No patients registered.");
        else popup->show("All Patients", info);
    }

    // ---- View All Doctors ----
    if (btnViewDoctors->isPressed()) {
        clearAction();
        char info[800]; strCopy(info,"");
        for (int i=0;i<ui->getDoctors()->size();i++) {
            Doctor* d = ui->getDoctors()->at(i);
            strCat(info, d->getID()); strCat(info," | Dr."); strCat(info, d->getName());
            strCat(info," | "); strCat(info, d->getSpecialization());
            strCat(info," | Rs."); char fstr[20]; floatToChar(fstr,d->getConsultationFee()); strCat(info,fstr); strCat(info,"\n");
        }
        contentTitle->setText("All Doctors:");
        if (ui->getDoctors()->size()==0) popup->show("Doctors","No doctors registered.");
        else popup->show("All Doctors", info);
    }

    // ---- View All Appointments ----
    if (btnViewAppts->isPressed()) {
        clearAction();
        char info[800]; strCopy(info,"");
        for (int i=0;i<ui->getAppointments()->size();i++) {
            Appointment* a = ui->getAppointments()->at(i);
            strCat(info, a->getID()); strCat(info," | P:"); strCat(info, a->getPatientID());
            strCat(info," D:"); strCat(info, a->getDoctorID()); strCat(info," | ");
            strCat(info, a->getDate()); strCat(info," "); strCat(info, a->getTimeSlot());
            strCat(info," | "); strCat(info, a->getStatus()); strCat(info,"\n");
        }
        contentTitle->setText("All Appointments:");
        if (ui->getAppointments()->size()==0) popup->show("Appointments","No appointments found.");
        else popup->show("All Appointments", info);
    }

    // ---- View Unpaid Bills ----
    if (btnViewBills->isPressed()) {
        clearAction();
        char info[800]; strCopy(info,"");
        float total=0; int cnt=0;
        for (int i=0;i<ui->getBills()->size();i++) {
            Bill* b = ui->getBills()->at(i);
            if (strCompare(b->getStatus(),"Unpaid")==0) {
                strCat(info, b->getID()); strCat(info," | P:"); strCat(info, b->getPatientID());
                strCat(info," | Rs."); char astr[20]; floatToChar(astr,b->getAmount()); strCat(info,astr); strCat(info,"\n");
                total+=b->getAmount(); cnt++;
            }
        }
        if (cnt==0) { popup->show("Unpaid Bills","No unpaid bills."); return; }
        strCat(info,"\nTotal Unpaid: Rs."); char tstr[20]; floatToChar(tstr,total); strCat(info,tstr);
        popup->show("Unpaid Bills", info);
    }

    // ---- Add Doctor ----
    if (btnAddDoctor->isPressed()) {
        contentTitle->setText("Add New Doctor");
        const char* lbls[]={ "Doctor ID (D###):","Full Name:","Specialization:","Contact (11 digits):","Password (min 6):","Consultation Fee:" };
        setMultiAction(10, lbls, 6, "Add Doctor");
    }
    if (activeAction==10 && submitBtn->isPressed()) {
        const char* did  = inputFields[0]->getText();
        const char* nm   = inputFields[1]->getText();
        const char* spec = inputFields[2]->getText();
        const char* cont = inputFields[3]->getText();
        const char* pass = inputFields[4]->getText();
        float fee = charToFloat(inputFields[5]->getText());

        if (!ui->getValidator()->validateID(did,"D"))        { popup->show("Error","Invalid Doctor ID (e.g. D001)"); return; }
        if (strLen(nm)==0)                                    { popup->show("Error","Name cannot be empty."); return; }
        if (strLen(spec)==0)                                  { popup->show("Error","Specialization cannot be empty."); return; }
        if (!ui->getValidator()->validateContact(cont))       { popup->show("Error","Contact must be 11 digits."); return; }
        if (!ui->getValidator()->validatePassword(pass))      { popup->show("Error","Password must be at least 6 chars."); return; }
        if (fee <= 0)                                         { popup->show("Error","Fee must be positive."); return; }
        if (ui->getDoctors()->findByID(did))                  { popup->show("Error","Doctor ID already exists."); return; }

        Doctor newDoc(did, nm, spec, cont, pass, fee);
        ui->getDoctors()->add(newDoc);
        char csv[512]; newDoc.toCSV(csv);
        ui->getFileHandler()->appendRecord("doctors.txt", csv);
        popup->show("Success","Doctor added successfully!");
        clearAction(); contentTitle->setText("Doctor added.");
    }

    // ---- Remove Doctor ----
    if (btnRemoveDoctor->isPressed()) {
        contentTitle->setText("Remove Doctor");
        const char* lbls[]={ "Enter Doctor ID to remove:" };
        setMultiAction(20, lbls, 1, "Remove Doctor");
    }
    if (activeAction==20 && submitBtn->isPressed()) {
        strCopy(tempDoctorID, inputFields[0]->getText());
        Doctor* d = ui->getDoctors()->findByID(tempDoctorID);
        if (!d) { popup->show("Error","Doctor not found."); return; }
        ui->getDoctors()->removeByID(tempDoctorID);
        ui->getFileHandler()->deleteRecordByID("doctors.txt", tempDoctorID);
        popup->show("Success","Doctor removed successfully.");
        clearAction(); contentTitle->setText("Doctor removed.");
    }

    // ---- Register Patient ----
    if (btnAddPatient->isPressed()) {
        contentTitle->setText("Register New Patient");
        const char* lbls[]={ "Patient ID (P###):","Full Name:","Age:","Gender (M/F):","Contact (11 digits):","Password (min 6):" };
        setMultiAction(40, lbls, 6, "Register");
    }
    if (activeAction==40 && submitBtn->isPressed()) {
        const char* pid  = inputFields[0]->getText();
        const char* nm   = inputFields[1]->getText();
        int age          = charToInt(inputFields[2]->getText());
        const char* genStr = inputFields[3]->getText();
        char gen         = strLen(genStr) > 0 ? genStr[0] : ' ';
        const char* cont = inputFields[4]->getText();
        const char* pass = inputFields[5]->getText();

        if (!ui->getValidator()->validateID(pid,"P"))         { popup->show("Error","Invalid Patient ID (e.g. P001)"); return; }
        if (strLen(nm)==0)                                    { popup->show("Error","Name cannot be empty."); return; }
        if (age <= 0 || age > 150)                            { popup->show("Error","Invalid age."); return; }
        if (gen != 'M' && gen != 'F' && gen != 'm' && gen != 'f') { popup->show("Error","Gender must be M or F."); return; }
        if (!ui->getValidator()->validateContact(cont))       { popup->show("Error","Contact must be 11 digits."); return; }
        if (!ui->getValidator()->validatePassword(pass))      { popup->show("Error","Password must be at least 6 chars."); return; }
        if (ui->getPatients()->findByID(pid))                 { popup->show("Error","Patient ID already exists."); return; }

        if (gen == 'm') gen = 'M';
        if (gen == 'f') gen = 'F';

        Patient newPat(pid, nm, age, gen, cont, pass, 0.0f);
        ui->getPatients()->add(newPat);
        char csv[512]; newPat.toCSV(csv);
        ui->getFileHandler()->appendRecord("patients.txt", csv);
        popup->show("Success","Patient registered successfully!");
        clearAction(); contentTitle->setText("Patient registered.");
    }

    // ---- Discharge Patient ----
    if (btnDischarge->isPressed()) {
        contentTitle->setText("Discharge Patient");
        const char* lbls[]={ "Enter Patient ID to discharge:" };
        setMultiAction(30, lbls, 1, "Discharge");
    }
    if (activeAction==30 && submitBtn->isPressed()) {
        strCopy(tempPatientID, inputFields[0]->getText());
        Patient* p = ui->getPatients()->findByID(tempPatientID);
        if (!p) { popup->show("Error","Patient not found."); return; }
        // Archive patient CSV
        char csv[512]; p->toCSV(csv);
        ui->getFileHandler()->appendDischarged(csv);
        // Cancel all pending appointments
        for (int i=0;i<ui->getAppointments()->size();i++) {
            Appointment* a = ui->getAppointments()->at(i);
            if (strCompare(a->getPatientID(), tempPatientID)==0 && strCompare(a->getStatus(),"Pending")==0)
                a->setStatus("Cancelled");
        }
        ui->getPatients()->removeByID(tempPatientID);
        ui->getFileHandler()->deleteRecordByID("patients.txt", tempPatientID);
        ui->getFileHandler()->rewriteAppointments(ui->getAppointments());
        popup->show("Discharged","Patient discharged and archived.");
        clearAction(); contentTitle->setText("Patient discharged.");
    }

    // ---- Security Log ----
    if (btnSecurityLog->isPressed()) {
        clearAction();
        // Read log file and display in popup
        char filepath[256]; strCopy(filepath,"data/security_log.txt");
        FILE* f = fopen(filepath,"r");
        char info[800]; strCopy(info,"");
        if (!f) { popup->show("Security Log","No log file found."); return; }
        char line[200];
        int lineCount=0;
        while (fgets(line, 200, f) && lineCount<8) {
            strCat(info, line); lineCount++;
        }
        fclose(f);
        if (strLen(info)==0) popup->show("Security Log","Log is empty.");
        else                  popup->show("Security Log (Last 8)", info);
    }

    // ---- Daily Report ----
    if (btnReport->isPressed()) {
        clearAction();
        int totalAppts=0, completedAppts=0, cancelledAppts=0;
        float totalRevenue=0;
        for (int i=0;i<ui->getAppointments()->size();i++) {
            Appointment* a = ui->getAppointments()->at(i);
            totalAppts++;
            if (strCompare(a->getStatus(),"Completed")==0) completedAppts++;
            if (strCompare(a->getStatus(),"Cancelled")==0) cancelledAppts++;
        }
        for (int i=0;i<ui->getBills()->size();i++) {
            Bill* b = ui->getBills()->at(i);
            if (strCompare(b->getStatus(),"Paid")==0) totalRevenue += b->getAmount();
        }
        char report[500];
        strCopy(report,"== DAILY REPORT ==\n");
        strCat(report,"Total Doctors: "); char nstr[10]; intToChar(nstr,ui->getDoctors()->size()); strCat(report,nstr); strCat(report,"\n");
        strCat(report,"Total Patients: "); intToChar(nstr,ui->getPatients()->size()); strCat(report,nstr); strCat(report,"\n");
        strCat(report,"Total Appointments: "); intToChar(nstr,totalAppts); strCat(report,nstr); strCat(report,"\n");
        strCat(report,"Completed: "); intToChar(nstr,completedAppts); strCat(report,nstr); strCat(report,"\n");
        strCat(report,"Cancelled: "); intToChar(nstr,cancelledAppts); strCat(report,nstr); strCat(report,"\n");
        strCat(report,"Total Revenue: Rs."); char rstr[20]; floatToChar(rstr,totalRevenue); strCat(report,rstr);
        popup->show("Daily Report", report);
    }

    if (btnLogout->isPressed()) {
        ui->logout();
        ui->getSceneManager()->changeScene(new LoginScreen());
    }
}

void AdminDashboard::draw(sf::RenderWindow& window) {
    sf::RectangleShape sidebar(sf::Vector2f(240,(float)window.getSize().y));
    sidebar.setFillColor(A_SIDEBAR); window.draw(sidebar);

    sf::RectangleShape header(sf::Vector2f((float)window.getSize().x,80));
    header.setFillColor(A_HDR); window.draw(header);

    welcomeLabel->draw(window);
    roleLabel->draw(window);
    window.draw(contentPanel);
    contentTitle->draw(window);

    btnViewPatients->draw(window); btnViewDoctors->draw(window);
    btnViewAppts->draw(window);    btnViewBills->draw(window);
    btnAddDoctor->draw(window);    btnRemoveDoctor->draw(window);
    btnAddPatient->draw(window);   btnDischarge->draw(window);    
    btnSecurityLog->draw(window);
    btnReport->draw(window);       btnLogout->draw(window);

    if (activeAction!=0) {
        for (int i=0;i<numFields;i++) {
            inputLabels[i]->draw(window);
            inputFields[i]->draw(window);
        }
        submitBtn->draw(window); cancelInputBtn->draw(window);
    }

    popup->draw(window);
}
