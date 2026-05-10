#include "../include/PatientDashboard.h"
#include "../include/LoginScreen.h"
#include "../include/UIManager.h"

//  colours 
static const sf::Color SIDEBAR_BG   (30,  39,  46);
static const sf::Color BTN_IDLE     (44,  62,  80);
static const sf::Color BTN_HOVER    (52, 152, 219);
static const sf::Color BTN_ACTIVE   (41, 128, 185);
static const sf::Color HEADER_BG    (41, 128, 185);
static const sf::Color CONTENT_BG   (245,248,250);

PatientDashboard::PatientDashboard()
    : currentUser(nullptr), welcomeLabel(nullptr), balanceLabel(nullptr),
      roleLabel(nullptr), btnViewAppts(nullptr), btnBookAppt(nullptr),
      btnCancelAppt(nullptr), btnMedRecords(nullptr), btnViewBills(nullptr),
      btnPayBill(nullptr), btnTopUp(nullptr), btnLogout(nullptr),
      contentTitle(nullptr), dropdownDoctors(nullptr), dropdownLabel(nullptr),
      confirmDocBtn(nullptr), inputField(nullptr), inputLabel(nullptr),
      submitBtn(nullptr), cancelInputBtn(nullptr), popup(nullptr),
      activeAction(0) {
    tempSpec[0] = tempDocID[0] = tempDate[0] = tempSlot[0] = '\0';
    tempApptID[0] = tempBillID[0] = '\0';
}

PatientDashboard::~PatientDashboard() {
    delete welcomeLabel; delete balanceLabel; delete roleLabel;
    delete btnViewAppts; delete btnBookAppt; delete btnCancelAppt;
    delete btnMedRecords; delete btnViewBills; delete btnPayBill;
    delete btnTopUp; delete btnLogout;
    delete contentTitle; delete dropdownDoctors; delete dropdownLabel;
    delete confirmDocBtn; delete inputField; delete inputLabel;
    delete submitBtn; delete cancelInputBtn; delete popup;
}

void PatientDashboard::init() {
    UIManager* ui = UIManager::getInstance();
    sf::Font&  font = ui->getFont();
    sf::Vector2u ws = ui->getWindow()->getSize();

    currentUser = ui->getPatients()->findByID(ui->getLoggedInUserID());

    // Header labels
    char welcome[150]; strCopy(welcome, "Welcome, "); strCat(welcome, currentUser->getName());
    welcomeLabel = new Label(260, 18, welcome, font, 24, sf::Color::White);
    roleLabel    = new Label(260, 48, "[ PATIENT PORTAL ]", font, 14, sf::Color(189,215,238));
    balanceLabel = new Label(700, 25, "Balance: Rs. 0.00", font, 18, sf::Color::White);
    refreshBalance();

    // Sidebar buttons (x=10, width=220, height=42)
    float sx=10, sw=220, sh=42, sy=120;
    float gap=8;
    btnViewAppts  = new Button(sx, sy,         sw, sh, "My Appointments",  font, BTN_IDLE, BTN_HOVER, BTN_ACTIVE); sy+=sh+gap;
    btnBookAppt   = new Button(sx, sy,         sw, sh, "Book Appointment", font, BTN_IDLE, BTN_HOVER, BTN_ACTIVE); sy+=sh+gap;
    btnCancelAppt = new Button(sx, sy,         sw, sh, "Cancel Appointment",font,BTN_IDLE, BTN_HOVER, BTN_ACTIVE); sy+=sh+gap;
    btnMedRecords = new Button(sx, sy,         sw, sh, "Medical Records",  font, BTN_IDLE, BTN_HOVER, BTN_ACTIVE); sy+=sh+gap;
    btnViewBills  = new Button(sx, sy,         sw, sh, "View Bills",       font, BTN_IDLE, BTN_HOVER, BTN_ACTIVE); sy+=sh+gap;
    btnPayBill    = new Button(sx, sy,         sw, sh, "Pay Bill",         font, BTN_IDLE, BTN_HOVER, BTN_ACTIVE); sy+=sh+gap;
    btnTopUp      = new Button(sx, sy,         sw, sh, "Top Up Balance",   font, BTN_IDLE, BTN_HOVER, BTN_ACTIVE); sy+=sh+gap+20;
    btnLogout     = new Button(sx, (float)ws.y-60, sw, sh, "Logout", font,
                               sf::Color(192,57,43), sf::Color(231,76,60), sf::Color(169,50,38));

    // Content area
    contentPanel.setPosition(242, 90);
    contentPanel.setSize(sf::Vector2f((float)ws.x - 242, (float)ws.y - 90));
    contentPanel.setFillColor(CONTENT_BG);

    contentTitle = new Label(260, 100, "Select an action from the sidebar.", font, 20, sf::Color(44,62,80));

    inputLabel  = new Label(260, 200, "", font, 17, sf::Color(44,62,80));
    inputField  = new InputBox(260, 228, 400, 42, "", font, false);
    submitBtn   = new Button(260, 282, 180, 40, "Submit", font, BTN_HOVER, BTN_ACTIVE, BTN_ACTIVE);
    cancelInputBtn = new Button(450, 282, 120, 40, "Cancel", font,
                                sf::Color(127,140,141), sf::Color(99,110,114), sf::Color(86,101,115));

    // Doctor dropdown (shown during booking step 11)
    dropdownLabel  = new Label(260, 160, "Select a Doctor:", font, 17, sf::Color(44,62,80));
    dropdownDoctors= new Dropdown(260, 185, 450, 42, "-- Select Doctor --", font);
    confirmDocBtn  = new Button(260, 390, 200, 42, "Select & Continue",
                                font, BTN_HOVER, BTN_ACTIVE, BTN_ACTIVE);

    popup = new Popup(font, ws);
}

void PatientDashboard::refreshBalance() {
    char msg[60]; strCopy(msg, "Balance: Rs. ");
    char bstr[20]; floatToChar(bstr, currentUser->getBalance()); strCat(msg, bstr);
    balanceLabel->setText(msg);
}

void PatientDashboard::setAction(int action, const char* lbl, const char* btn) {
    activeAction = action;
    inputLabel->setText(lbl);
    submitBtn->setText(btn);
    inputField->clear();
}

void PatientDashboard::clearAction() {
    activeAction = 0;
    inputField->clear();
}


void PatientDashboard::handleEvent(sf::Event& event, sf::RenderWindow& window) {
    if (popup->isVisible()) { popup->handleEvent(event, window); return; }

    // Sidebar
    btnViewAppts->handleEvent(event, window);
    btnBookAppt->handleEvent(event, window);
    btnCancelAppt->handleEvent(event, window);
    btnMedRecords->handleEvent(event, window);
    btnViewBills->handleEvent(event, window);
    btnPayBill->handleEvent(event, window);
    btnTopUp->handleEvent(event, window);
    btnLogout->handleEvent(event, window);

    // Input field
    if (activeAction != 0) {
        if (activeAction != 11) {
            inputField->handleEvent(event, window);
        } else {
            dropdownDoctors->handleEvent(event, window);
            confirmDocBtn->handleEvent(event, window);
        }
        submitBtn->handleEvent(event, window);
        cancelInputBtn->handleEvent(event, window);
    }
}


void PatientDashboard::update(float dt) {
    if (popup->isVisible()) { popup->update(dt); return; }

    UIManager* ui = UIManager::getInstance();

    // Sidebar updates
    btnViewAppts->update(dt);  btnBookAppt->update(dt);  btnCancelAppt->update(dt);
    btnMedRecords->update(dt); btnViewBills->update(dt); btnPayBill->update(dt);
    btnTopUp->update(dt);      btnLogout->update(dt);

    if (activeAction != 0) {
        if (activeAction != 11) {
            inputField->update(dt);
            submitBtn->update(dt);
        } else {
            dropdownDoctors->update(dt);
            confirmDocBtn->update(dt);
        }
        cancelInputBtn->update(dt);
        if (cancelInputBtn->isPressed()) { clearAction(); contentTitle->setText("Cancelled."); return; }
    }

    // ---- Sidebar button presses ----
    if (btnViewAppts->isPressed()) {
        clearAction();
        // Count appointments
        int cnt = 0;
        char info[800]; strCopy(info, "");
        for (int i = 0; i < ui->getAppointments()->size(); i++) {
            Appointment* a = ui->getAppointments()->at(i);
            if (strCompare(a->getPatientID(), currentUser->getID()) == 0) {
                strCat(info, a->getID()); strCat(info, " | ");
                strCat(info, a->getDate()); strCat(info, " ");
                strCat(info, a->getTimeSlot()); strCat(info, " | Dr.");
                strCat(info, a->getDoctorID()); strCat(info, " | ");
                strCat(info, a->getStatus()); strCat(info, "\n");
                cnt++;
            }
        }
        contentTitle->setText("Your Appointments:");
        if (cnt == 0) popup->show("Appointments", "No appointments found.");
        else          popup->show("Your Appointments", info);
    }

    if (btnViewBills->isPressed()) {
        clearAction();
        char info[800]; strCopy(info, "");
        int cnt = 0;
        float unpaid = 0;
        for (int i = 0; i < ui->getBills()->size(); i++) {
            Bill* b = ui->getBills()->at(i);
            if (strCompare(b->getPatientID(), currentUser->getID()) == 0) {
                strCat(info, b->getID()); strCat(info, " | Rs.");
                char bstr[20]; floatToChar(bstr, b->getAmount()); strCat(info, bstr);
                strCat(info, " | "); strCat(info, b->getStatus()); strCat(info, "\n");
                if (strCompare(b->getStatus(), "Unpaid") == 0) unpaid += b->getAmount();
                cnt++;
            }
        }
        if (cnt == 0) { popup->show("Bills", "No bills found."); return; }
        char totalMsg[900]; strCopy(totalMsg, info);
        strCat(totalMsg, "\nTotal Unpaid: Rs.");
        char ustr[20]; floatToChar(ustr, unpaid); strCat(totalMsg, ustr);
        popup->show("Your Bills", totalMsg);
    }

    if (btnMedRecords->isPressed()) {
        clearAction();
        char info[800]; strCopy(info, "");
        int cnt = 0;
        for (int i = 0; i < ui->getPrescriptions()->size(); i++) {
            Prescription* p = ui->getPrescriptions()->at(i);
            if (strCompare(p->getPatientID(), currentUser->getID()) == 0) {
                strCat(info, p->getID()); strCat(info, " | ");
                strCat(info, p->getDate()); strCat(info, "\nMeds: ");
                strCat(info, p->getMedicines()); strCat(info, "\n\n");
                cnt++;
            }
        }
        if (cnt == 0) popup->show("Medical Records", "No medical records found.");
        else          popup->show("Medical Records", info);
    }

    // -- Book Appointment: Directly show ALL doctors in dropdown --
    if (btnBookAppt->isPressed()) {
        dropdownDoctors->clearItems();
        dropdownDoctors->clearSelection();
        int count = 0;
        for (int i = 0; i < ui->getDoctors()->size(); i++) {
            Doctor* d = ui->getDoctors()->at(i);
            char display[150];
            strCopy(display, d->getID()); strCat(display, " - Dr. ");
            strCat(display, d->getName()); strCat(display, " | ");
            strCat(display, d->getSpecialization()); strCat(display, " | Fee: Rs.");
            char fstr[20]; floatToChar(fstr, d->getConsultationFee()); strCat(display, fstr);
            dropdownDoctors->addItem(display, d->getID());
            count++;
        }
        if (count == 0) { popup->show("No Doctors", "No doctors are registered yet."); return; }
        contentTitle->setText("Book Appointment - Select a Doctor");
        activeAction = 11;
    }
    // Step: doctor selected from dropdown
    if (activeAction == 11 && confirmDocBtn->isPressed()) {
        if (!dropdownDoctors->hasSelection()) { popup->show("Error", "Please select a doctor first."); return; }
        strCopy(tempDocID, dropdownDoctors->getSelectedValue());
        Doctor* d = ui->getDoctors()->findByID(tempDocID);
        if (!d) { popup->show("Error", "Doctor not found."); return; }
        contentTitle->setText("Book Appointment - Enter Date (DD-MM-YYYY)");
        setAction(12, "Enter appointment date (DD-MM-YYYY):", "Next");
    }
    // Step 3: date
    if (activeAction == 12 && submitBtn->isPressed()) {
        strCopy(tempDate, inputField->getText());
        if (!ui->getValidator()->validateDate(tempDate)) { popup->show("Error", "Invalid date. Use DD-MM-YYYY (today or future)."); return; }
        // Show available slots
        char slotInfo[300]; strCopy(slotInfo, "Available Slots:\n");
        char validSlots[8][6]; ui->getValidator()->getValidSlots(validSlots);
        bool taken[8]; for (int s=0;s<8;s++) taken[s]=false;
        for (int i=0;i<ui->getAppointments()->size();i++) {
            Appointment* a = ui->getAppointments()->at(i);
            if (strCompare(a->getDoctorID(),tempDocID)==0 && strCompare(a->getDate(),tempDate)==0 && strCompare(a->getStatus(),"Cancelled")!=0) {
                for (int s=0;s<8;s++) if (strCompare(a->getTimeSlot(),validSlots[s])==0) taken[s]=true;
            }
        }
        for (int s=0;s<8;s++) if (!taken[s]) { strCat(slotInfo, validSlots[s]); strCat(slotInfo, "\n"); }
        popup->show("Available Slots", slotInfo);
        contentTitle->setText("Book Appointment - Step 4: Enter Time Slot (e.g. 09:00)");
        setAction(13, "Enter time slot (e.g. 09:00):", "Confirm Booking");
    }
    // Step 4: slot + booking
    if (activeAction == 13 && submitBtn->isPressed()) {
        strCopy(tempSlot, inputField->getText());
        if (!ui->getValidator()->validateTimeSlot(tempSlot)) { popup->show("Error", "Invalid time slot."); return; }
        Doctor* doc = ui->getDoctors()->findByID(tempDocID);
        if (!doc) { popup->show("Error", "Doctor not found."); clearAction(); return; }
        float fee = doc->getConsultationFee();
        if (currentUser->getBalance() < fee) {
            char msg[100]; strCopy(msg,"Insufficient balance. Need Rs."); char fstr[20]; floatToChar(fstr,fee); strCat(msg,fstr);
            popup->show("Insufficient Funds", msg); clearAction(); return;
        }
        // Check slot available
        bool avail = true;
        for (int i=0;i<ui->getAppointments()->size();i++) {
            Appointment* a = ui->getAppointments()->at(i);
            if (strCompare(a->getDoctorID(),tempDocID)==0 && strCompare(a->getDate(),tempDate)==0 &&
                strCompare(a->getTimeSlot(),tempSlot)==0 && strCompare(a->getStatus(),"Cancelled")!=0) { avail=false; break; }
        }
        if (!avail) { popup->show("Error","Slot already booked!"); return; }
        // Create appointment + bill
        generateNextID(tempApptID, "A", ui->getAppointments()->size()+1);
        generateNextID(tempBillID, "B", ui->getBills()->size()+1);
        *currentUser -= fee;
        // Update in storage
        Patient* sp = ui->getPatients()->findByID(currentUser->getID());
        if (sp) sp->setBalance(currentUser->getBalance());

        Appointment newA(tempApptID, currentUser->getID(), tempDocID, tempDate, tempSlot, "Pending");
        Bill        newB(tempBillID, currentUser->getID(), tempApptID, fee, "Unpaid", tempDate);
        ui->getAppointments()->add(newA); ui->getBills()->add(newB);

        char csvBuf[512];
        newA.toCSV(csvBuf); ui->getFileHandler()->appendRecord("appointments.txt", csvBuf);
        newB.toCSV(csvBuf); ui->getFileHandler()->appendRecord("bills.txt", csvBuf);
        currentUser->toCSV(csvBuf); ui->getFileHandler()->updateRecordByID("patients.txt", currentUser->getID(), csvBuf);

        refreshBalance();
        char ok[150]; strCopy(ok,"Appointment ID: "); strCat(ok,tempApptID); strCat(ok,"\nBill ID: "); strCat(ok,tempBillID);
        strCat(ok,"\nFee deducted: Rs."); char fstr[20]; floatToChar(fstr,fee); strCat(ok,fstr);
        popup->show("Booking Successful!", ok);
        clearAction(); contentTitle->setText("Booking complete.");
    }

    //  Cancel Appointment 
    if (btnCancelAppt->isPressed()) {
        contentTitle->setText("Cancel Appointment");
        setAction(20, "Enter Appointment ID to cancel:", "Cancel Appointment");
    }
    if (activeAction == 20 && submitBtn->isPressed()) {
        strCopy(tempApptID, inputField->getText());
        Appointment* appt = ui->getAppointments()->findByID(tempApptID);
        if (!appt) { popup->show("Error","Appointment not found."); return; }
        if (strCompare(appt->getPatientID(), currentUser->getID())!=0) { popup->show("Error","This appointment is not yours."); return; }
        if (strCompare(appt->getStatus(),"Pending")!=0) { popup->show("Error","Only Pending appointments can be cancelled."); return; }
        appt->setStatus("Cancelled");
        // refund
        for (int i=0;i<ui->getBills()->size();i++) {
            Bill* b = ui->getBills()->at(i);
            if (strCompare(b->getAppointmentID(), tempApptID)==0) {
                *currentUser += b->getAmount();
                Patient* sp = ui->getPatients()->findByID(currentUser->getID());
                if (sp) sp->setBalance(currentUser->getBalance());
                b->setStatus("Refunded"); break;
            }
        }
        ui->getFileHandler()->rewriteAppointments(ui->getAppointments());
        ui->getFileHandler()->rewriteBills(ui->getBills());
        ui->getFileHandler()->rewritePatients(ui->getPatients());
        refreshBalance();
        popup->show("Cancelled","Appointment cancelled and refund applied.");
        clearAction(); contentTitle->setText("Appointment cancelled.");
    }

    // -- Pay Bill --
    if (btnPayBill->isPressed()) {
        contentTitle->setText("Pay Bill");
        setAction(30, "Enter Bill ID to pay:", "Pay Now");
    }
    if (activeAction == 30 && submitBtn->isPressed()) {
        strCopy(tempBillID, inputField->getText());
        Bill* bill = ui->getBills()->findByID(tempBillID);
        if (!bill) { popup->show("Error","Bill not found."); return; }
        if (strCompare(bill->getPatientID(), currentUser->getID())!=0) { popup->show("Error","This bill is not yours."); return; }
        if (strCompare(bill->getStatus(),"Unpaid")!=0) { popup->show("Error","Bill is already paid or refunded."); return; }
        if (currentUser->getBalance() < bill->getAmount()) { popup->show("Error","Insufficient balance."); return; }
        *currentUser -= bill->getAmount();
        bill->setStatus("Paid");
        Patient* sp = ui->getPatients()->findByID(currentUser->getID());
        if (sp) sp->setBalance(currentUser->getBalance());
        ui->getFileHandler()->rewriteBills(ui->getBills());
        ui->getFileHandler()->rewritePatients(ui->getPatients());
        refreshBalance();
        popup->show("Success","Bill paid successfully!");
        clearAction(); contentTitle->setText("Bill payment complete.");
    }

    //  Top Up Balance 
    if (btnTopUp->isPressed()) {
        contentTitle->setText("Top Up Balance");
        setAction(40, "Enter amount to add (Rs.):", "Top Up");
    }
    if (activeAction == 40 && submitBtn->isPressed()) {
        float amount = charToFloat(inputField->getText());
        if (amount <= 0) { popup->show("Error","Enter a positive amount."); return; }
        *currentUser += amount;
        Patient* sp = ui->getPatients()->findByID(currentUser->getID());
        if (sp) sp->setBalance(currentUser->getBalance());
        ui->getFileHandler()->rewritePatients(ui->getPatients());
        refreshBalance();
        char msg[60]; strCopy(msg,"Balance added! New Balance: Rs.");
        char bstr[20]; floatToChar(bstr, currentUser->getBalance()); strCat(msg,bstr);
        popup->show("Top Up Successful",msg);
        clearAction(); contentTitle->setText("Balance updated.");
    }

    // -- Logout --
    if (btnLogout->isPressed()) {
        ui->logout();
        ui->getSceneManager()->changeScene(new LoginScreen());
    }
}


void PatientDashboard::draw(sf::RenderWindow& window) {
    // Sidebar
    sf::RectangleShape sidebar(sf::Vector2f(240, (float)window.getSize().y));
    sidebar.setFillColor(SIDEBAR_BG); window.draw(sidebar);

    // Sidebar label
    sf::Text sideLabel;
    sideLabel.setString("Navigation");
    window.draw(sidebar);

    // Header bar
    sf::RectangleShape header(sf::Vector2f((float)window.getSize().x, 80));
    header.setFillColor(HEADER_BG); window.draw(header);

    welcomeLabel->draw(window);
    roleLabel->draw(window);
    balanceLabel->draw(window);

    window.draw(contentPanel);
    contentTitle->draw(window);

    btnViewAppts->draw(window);  btnBookAppt->draw(window);
    btnCancelAppt->draw(window); btnMedRecords->draw(window);
    btnViewBills->draw(window);  btnPayBill->draw(window);
    btnTopUp->draw(window);      btnLogout->draw(window);

    if (activeAction != 0) {
        if (activeAction == 11) {
            // Show dropdown instead of text input
            dropdownLabel->draw(window);
            dropdownDoctors->draw(window);
            confirmDocBtn->draw(window);
            cancelInputBtn->draw(window);
        } else {
            inputLabel->draw(window);
            inputField->draw(window);
            submitBtn->draw(window);
            cancelInputBtn->draw(window);
        }
    }

    popup->draw(window);
}
