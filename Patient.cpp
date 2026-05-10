
// Patient.cpp - Patient class implementation
// MediCore Hospital Management System

#include "../include/Patient.h"
#include "../include/Doctor.h"
#include "../include/Appointment.h"
#include "../include/Bill.h"
#include "../include/Prescription.h"
#include "../include/FileHandler.h"
#include "../include/Validator.h"
#include "../include/HospitalException.h"

using namespace std;

// Default constructor
Patient::Patient() : Person(), age(0), gender('M'), balance(0.0f) {}

// Parameterized constructor
Patient::Patient(const char* id, const char* name, int age, char gender,
                 const char* contact, const char* password, float balance)
    : Person(id, name, contact, password), age(age), gender(gender), balance(balance) {}

// Copy constructor
Patient::Patient(const Patient& other)
    : Person(other), age(other.age), gender(other.gender), balance(other.balance) {}

// Assignment operator
Patient& Patient::operator=(const Patient& other) {
    if (this != &other) {
        Person::operator=(other);
        age = other.age;
        gender = other.gender;
        balance = other.balance;
    }
    return *this;
}

// Destructor
Patient::~Patient() {}

// Getters
int Patient::getAge() const { return age; }
char Patient::getGender() const { return gender; }
float Patient::getBalance() const { return balance; }

// Setters
void Patient::setAge(int a) { age = a; }
void Patient::setGender(char g) { gender = g; }
void Patient::setBalance(float b) { balance = b; }

// Operator += (add balance)
Patient& Patient::operator+=(float amount) {
    balance += amount;
    return *this;
}

// Operator -= (deduct balance)
Patient& Patient::operator-=(float amount) {
    balance -= amount;
    return *this;
}

// Operator == (compare by patient ID)
bool Patient::operator==(const Patient& other) const {
    return strCompare(id, other.id) == 0;
}

// Operator << (formatted display)
ostream& operator<<(ostream& os, const Patient& p) {
    char balStr[20];
    floatToChar(balStr, p.balance);
    os << "+===========================================+\n";
    os << "|           PATIENT INFORMATION             |\n";
    os << "+===========================================+\n";
    os << "| Patient ID : " << p.id << "\n";
    os << "| Name       : " << p.name << "\n";
    os << "| Age        : " << p.age << "\n";
    os << "| Gender     : " << p.gender << "\n";
    os << "| Contact    : " << p.contact << "\n";
    os << "| Balance    : Rs. " << balStr << "\n";
    os << "+===========================================+\n";
    return os;
}

// Display (virtual)
void Patient::display() const {
    cout << *this;
}

// Get role (virtual)
const char* Patient::getRole() const {
    return "Patient";
}

// Login
bool Patient::login(const char* inputID, const char* inputPassword) {
    return (strCompare(id, inputID) == 0 && strCompare(password, inputPassword) == 0);
}

// Convert to CSV: patient_id,name,age,gender,contact,password,balance
void Patient::toCSV(char* buffer) const {
    char ageStr[10], balStr[20], genStr[2];
    intToChar(ageStr, age);
    floatToChar(balStr, balance);
    genStr[0] = gender; genStr[1] = '\0';

    strCopy(buffer, id); strCat(buffer, ",");
    strCat(buffer, name); strCat(buffer, ",");
    strCat(buffer, ageStr); strCat(buffer, ",");
    strCat(buffer, genStr); strCat(buffer, ",");
    strCat(buffer, contact); strCat(buffer, ",");
    strCat(buffer, password); strCat(buffer, ",");
    strCat(buffer, balStr);
}

 
// BOOK APPOINTMENT

void Patient::bookAppointment(Storage<Doctor>* doctors, Storage<Appointment>* appointments,
                              Storage<Bill>* bills, FileHandler* fh, Validator* val) {
    try {
        cout << "\n+===========================================+\n";
        cout << "|         BOOK AN APPOINTMENT              |\n";
        cout << "+===========================================+\n";

        // Search doctor by specialization
        char specSearch[100];
        cout << "  Enter specialization to search: ";
        cin.ignore();
        cin.getline(specSearch, 100);

        if (!val->validateNonEmpty(specSearch)) {
            throw InvalidInputException("Specialization cannot be empty.");
        }

        // Find matching doctors (case-insensitive)
        cout << "\n  Available Doctors:\n";
        cout << "  ----------------------------------\n";
        int foundCount = 0;
        for (int i = 0; i < doctors->size(); i++) {
            Doctor* d = doctors->at(i);
            if (strContainsCI(d->getSpecialization(), specSearch)) {
                char feeStr[20];
                floatToChar(feeStr, d->getConsultationFee());
                cout << "  " << (foundCount + 1) << ". " << d->getID()
                          << " | Dr. " << d->getName()
                          << " | " << d->getSpecialization()
                          << " | Fee: Rs. " << feeStr << "\n";
                foundCount++;
            }
        }

        if (foundCount == 0) {
            cout << "  No doctors found with that specialization.\n";
            return;
        }

        // Select doctor
        char docID[30];
        cout << "\n  Enter Doctor ID: ";
        cin.getline(docID, 30);

        Doctor* selectedDoc = doctors->findByID(docID);
        if (!selectedDoc) {
            throw InvalidInputException("Doctor not found.");
        }

        // Verify specialization matches
        if (!strContainsCI(selectedDoc->getSpecialization(), specSearch)) {
            throw InvalidInputException("Doctor specialization does not match search.");
        }

        // Enter and validate date
        char dateStr[15];
        cout << "  Enter appointment date (DD-MM-YYYY): ";
        cin.getline(dateStr, 15);

        if (!val->validateDate(dateStr)) {
            throw InvalidInputException("Invalid date. Use DD-MM-YYYY format (today or future).");
        }

        // Show available time slots
        char validSlots[8][6];
        val->getValidSlots(validSlots);

        cout << "\n  Available Time Slots:\n";
        cout << "  ----------------------------------\n";
        int availableCount = 0;
        bool slotAvailable[8];
        for (int s = 0; s < Validator::NUM_SLOTS; s++) {
            slotAvailable[s] = true;
        }

        // Check which slots are taken
        for (int i = 0; i < appointments->size(); i++) {
            Appointment* appt = appointments->at(i);
            if (strCompare(appt->getDoctorID(), docID) == 0 &&
                strCompare(appt->getDate(), dateStr) == 0 &&
                strCompare(appt->getStatus(), "Cancelled") != 0) {
                for (int s = 0; s < Validator::NUM_SLOTS; s++) {
                    if (strCompare(appt->getTimeSlot(), validSlots[s]) == 0) {
                        slotAvailable[s] = false;
                    }
                }
            }
        }

        for (int s = 0; s < Validator::NUM_SLOTS; s++) {
            if (slotAvailable[s]) {
                cout << "  " << (availableCount + 1) << ". " << validSlots[s] << "\n";
                availableCount++;
            }
        }

        if (availableCount == 0) {
            throw SlotUnavailableException("No available slots for this doctor on selected date.");
        }

        // Select time slot
        char selectedSlot[10];
        cout << "\n  Enter time slot (e.g., 09:00): ";
        cin.getline(selectedSlot, 10);

        if (!val->validateTimeSlot(selectedSlot)) {
            throw InvalidInputException("Invalid time slot.");
        }

        // Check if the slot is actually available
        bool isAvail = false;
        for (int s = 0; s < Validator::NUM_SLOTS; s++) {
            if (strCompare(selectedSlot, validSlots[s]) == 0 && slotAvailable[s]) {
                isAvail = true;
                break;
            }
        }
        if (!isAvail) {
            throw SlotUnavailableException("Selected slot is not available.");
        }

        // Check scheduling conflict using operator==
        Appointment tempAppt("TEMP", id, docID, dateStr, selectedSlot, "Pending");
        for (int i = 0; i < appointments->size(); i++) {
            if (tempAppt == *(appointments->at(i))) {
                throw SlotUnavailableException("Scheduling conflict detected!");
            }
        }

        // Check balance
        float fee = selectedDoc->getConsultationFee();
        if (balance < fee) {
            char msg[200];
            strCopy(msg, "Insufficient balance. Required: Rs. ");
            char feeStr[20];
            floatToChar(feeStr, fee);
            strCat(msg, feeStr);
            throw InsufficientFundsException(msg);
        }

        // Generate appointment ID
        char apptID[20];
        generateNextID(apptID, "A", appointments->size() + 1);

        // Generate bill ID
        char billID[20];
        generateNextID(billID, "B", bills->size() + 1);

        // Deduct fee
        *this -= fee;

        // Create appointment
        Appointment newAppt(apptID, id, docID, dateStr, selectedSlot, "Pending");
        appointments->add(newAppt);

        // Create bill
        Bill newBill(billID, id, apptID, fee, "Unpaid", dateStr);
        bills->add(newBill);

        // Save to files
        char csvBuf[512];
        newAppt.toCSV(csvBuf);
        fh->appendRecord("appointments.txt", csvBuf);

        newBill.toCSV(csvBuf);
        fh->appendRecord("bills.txt", csvBuf);

        // Update patient balance in file
        toCSV(csvBuf);
        fh->updateRecordByID("patients.txt", id, csvBuf);

        cout << "\n  [SUCCESS] Appointment booked successfully!\n";
        cout << "  Appointment ID: " << apptID << "\n";
        cout << "  Bill ID: " << billID << "\n";
        char balStr[20];
        floatToChar(balStr, balance);
        cout << "  Remaining Balance: Rs. " << balStr << "\n";

    } catch (const HospitalException& e) {
        cout << "\n  [ERROR] " << e.what() << "\n";
    }
}

// CANCEL APPOINTMENT

void Patient::cancelAppointment(Storage<Appointment>* appointments, Storage<Bill>* bills,
                                Storage<Patient>* patients, FileHandler* fh, Validator* val) {
    try {
        cout << "\n+===========================================+\n";
        cout << "|        CANCEL AN APPOINTMENT             |\n";
        cout << "+===========================================+\n";

        // Show pending appointments for this patient
        int found = 0;
        for (int i = 0; i < appointments->size(); i++) {
            Appointment* a = appointments->at(i);
            if (strCompare(a->getPatientID(), id) == 0 &&
                strCompare(a->getStatus(), "Pending") == 0) {
                cout << *a;
                found++;
            }
        }

        if (found == 0) {
            cout << "  No pending appointments to cancel.\n";
            return;
        }

        char apptID[30];
        cout << "\n  Enter Appointment ID to cancel: ";
        cin.ignore();
        cin.getline(apptID, 30);

        Appointment* appt = appointments->findByID(apptID);
        if (!appt) {
            throw InvalidInputException("Appointment not found.");
        }

        // Validate ownership
        if (strCompare(appt->getPatientID(), id) != 0) {
            throw InvalidInputException("This appointment does not belong to you.");
        }

        if (strCompare(appt->getStatus(), "Pending") != 0) {
            throw InvalidInputException("Only pending appointments can be cancelled.");
        }

        // Find corresponding bill and refund
        for (int i = 0; i < bills->size(); i++) {
            Bill* b = bills->at(i);
            if (strCompare(b->getAppointmentID(), apptID) == 0) {
                // Refund using +=
                *this += b->getAmount();
                b->setStatus("Refunded");
                break;
            }
        }

        // Update appointment status
        appt->setStatus("Cancelled");

        // Save changes to files
        fh->rewriteAppointments(appointments);
        fh->rewriteBills(bills);

        // Update patient in storage and file
        Patient* self = patients->findByID(id);
        if (self) {
            self->setBalance(balance);
        }
        fh->rewritePatients(patients);

        cout << "\n  [SUCCESS] Appointment cancelled. Refund applied.\n";
        char balStr[20];
        floatToChar(balStr, balance);
        cout << "  Updated Balance: Rs. " << balStr << "\n";

    } catch (const HospitalException& e) {
        cout << "\n  [ERROR] " << e.what() << "\n";
    }
}


// VIEW APPOINTMENTS (sorted by date ascending - bubble sort)

void Patient::viewAppointments(Storage<Appointment>* appointments) {
    cout << "\n+===========================================+\n";
    cout << "|         YOUR APPOINTMENTS                |\n";
    cout << "+===========================================+\n";

    // Collect patient's appointments into a temp array
    Appointment* tempArr = new Appointment[100];
    int count = 0;

    for (int i = 0; i < appointments->size(); i++) {
        Appointment* a = appointments->at(i);
        if (strCompare(a->getPatientID(), id) == 0) {
            *(tempArr + count) = *a;
            count++;
        }
    }

    if (count == 0) {
        cout << "  No appointments found.\n";
        delete[] tempArr;
        return;
    }

    // Bubble sort by date ascending (DD-MM-YYYY -> compare YYYY then MM then DD)
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            const char* d1 = (tempArr + j)->getDate();
            const char* d2 = (tempArr + j + 1)->getDate();
            // Compare year first (pos 6-9), then month (3-4), then day (0-1)
            int y1 = charToInt(d1 + 6), y2 = charToInt(d2 + 6);
            int m1 = (*(d1 + 3) - '0') * 10 + (*(d1 + 4) - '0');
            int m2 = (*(d2 + 3) - '0') * 10 + (*(d2 + 4) - '0');
            int day1 = (*(d1) - '0') * 10 + (*(d1 + 1) - '0');
            int day2 = (*(d2) - '0') * 10 + (*(d2 + 1) - '0');

            bool shouldSwap = false;
            if (y1 > y2) shouldSwap = true;
            else if (y1 == y2 && m1 > m2) shouldSwap = true;
            else if (y1 == y2 && m1 == m2 && day1 > day2) shouldSwap = true;

            if (shouldSwap) {
                Appointment temp = *(tempArr + j);
                *(tempArr + j) = *(tempArr + j + 1);
                *(tempArr + j + 1) = temp;
            }
        }
    }

    for (int i = 0; i < count; i++) {
        cout << *(tempArr + i);
    }
    delete[] tempArr;
}


// VIEW MEDICAL RECORDS (grouped by appointment, sorted descending)

void Patient::viewMedicalRecords(Storage<Prescription>* prescriptions, Storage<Appointment>* appointments) {
    cout << "\n+===========================================+\n";
    cout << "|         YOUR MEDICAL RECORDS             |\n";
    cout << "+===========================================+\n";

    // Collect prescriptions for this patient
    Prescription* tempArr = new Prescription[100];
    int count = 0;

    for (int i = 0; i < prescriptions->size(); i++) {
        Prescription* p = prescriptions->at(i);
        if (strCompare(p->getPatientID(), id) == 0) {
            *(tempArr + count) = *p;
            count++;
        }
    }

    if (count == 0) {
        cout << "  No medical records found.\n";
        delete[] tempArr;
        return;
    }

    // Bubble sort by date descending
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            const char* d1 = (tempArr + j)->getDate();
            const char* d2 = (tempArr + j + 1)->getDate();
            int y1 = charToInt(d1 + 6), y2 = charToInt(d2 + 6);
            int m1 = (*(d1 + 3) - '0') * 10 + (*(d1 + 4) - '0');
            int m2 = (*(d2 + 3) - '0') * 10 + (*(d2 + 4) - '0');
            int day1 = (*(d1) - '0') * 10 + (*(d1 + 1) - '0');
            int day2 = (*(d2) - '0') * 10 + (*(d2 + 1) - '0');

            bool shouldSwap = false;
            if (y1 < y2) shouldSwap = true;
            else if (y1 == y2 && m1 < m2) shouldSwap = true;
            else if (y1 == y2 && m1 == m2 && day1 < day2) shouldSwap = true;

            if (shouldSwap) {
                Prescription temp = *(tempArr + j);
                *(tempArr + j) = *(tempArr + j + 1);
                *(tempArr + j + 1) = temp;
            }
        }
    }

    for (int i = 0; i < count; i++) {
        // Show appointment info
        Appointment* appt = appointments->findByID((tempArr + i)->getAppointmentID());
        if (appt) {
            cout << "  [Appointment: " << appt->getID() << " | "
                      << appt->getDate() << " | " << appt->getStatus() << "]\n";
        }
        cout << *(tempArr + i);
    }
    delete[] tempArr;
}


// VIEW BILLS

void Patient::viewBills(Storage<Bill>* bills) {
    cout << "\n+===========================================+\n";
    cout << "|            YOUR BILLS                    |\n";
    cout << "+===========================================+\n";

    float unpaidTotal = 0.0f;
    int found = 0;

    for (int i = 0; i < bills->size(); i++) {
        Bill* b = bills->at(i);
        if (strCompare(b->getPatientID(), id) == 0) {
            cout << *b;
            if (strCompare(b->getStatus(), "Unpaid") == 0) {
                unpaidTotal += b->getAmount();
            }
            found++;
        }
    }

    if (found == 0) {
        cout << "  No bills found.\n";
        return;
    }

    char totalStr[20];
    floatToChar(totalStr, unpaidTotal);
    cout << "\n  Total Unpaid: Rs. " << totalStr << "\n";
}

// PAY BILL

void Patient::payBill(Storage<Bill>* bills, Storage<Patient>* patients,
                      FileHandler* fh, Validator* val) {
    try {
        cout << "\n+===========================================+\n";
        cout << "|             PAY A BILL                   |\n";
        cout << "+===========================================+\n";

        // Show unpaid bills
        int found = 0;
        for (int i = 0; i < bills->size(); i++) {
            Bill* b = bills->at(i);
            if (strCompare(b->getPatientID(), id) == 0 &&
                strCompare(b->getStatus(), "Unpaid") == 0) {
                cout << *b;
                found++;
            }
        }

        if (found == 0) {
            cout << "  No unpaid bills.\n";
            return;
        }

        char billID[30];
        cout << "\n  Enter Bill ID to pay: ";
        cin.ignore();
        cin.getline(billID, 30);

        Bill* bill = bills->findByID(billID);
        if (!bill) {
            throw InvalidInputException("Bill not found.");
        }

        if (strCompare(bill->getPatientID(), id) != 0) {
            throw InvalidInputException("This bill does not belong to you.");
        }

        if (strCompare(bill->getStatus(), "Unpaid") != 0) {
            throw InvalidInputException("This bill is already paid or refunded.");
        }

        if (balance < bill->getAmount()) {
            throw InsufficientFundsException("Insufficient balance to pay this bill.");
        }

        // Deduct and update
        *this -= bill->getAmount();
        bill->setStatus("Paid");

        // Update storage
        Patient* self = patients->findByID(id);
        if (self) self->setBalance(balance);

        // Save to files
        fh->rewriteBills(bills);
        fh->rewritePatients(patients);

        cout << "\n  [SUCCESS] Bill paid successfully!\n";
        char balStr[20];
        floatToChar(balStr, balance);
        cout << "  Remaining Balance: Rs. " << balStr << "\n";

    } catch (const HospitalException& e) {
        cout << "\n  [ERROR] " << e.what() << "\n";
    }
}

// TOP UP BALANCE

void Patient::topUpBalance(Storage<Patient>* patients, FileHandler* fh, Validator* val) {
    try {
        cout << "\n+===========================================+\n";
        cout << "|           TOP UP BALANCE                 |\n";
        cout << "+===========================================+\n";

        char balStr[20];
        floatToChar(balStr, balance);
        cout << "  Current Balance: Rs. " << balStr << "\n";

        char amtStr[20];
        cout << "  Enter amount to add: Rs. ";
        cin.ignore();
        cin.getline(amtStr, 20);

        float amount = charToFloat(amtStr);
        if (!val->validatePositiveFloat(amount)) {
            throw InvalidInputException("Amount must be a positive number.");
        }

        *this += amount;

        // Update storage
        Patient* self = patients->findByID(id);
        if (self) self->setBalance(balance);

        // Save to file
        fh->rewritePatients(patients);

        floatToChar(balStr, balance);
        cout << "\n  [SUCCESS] Balance updated!\n";
        cout << "  New Balance: Rs. " << balStr << "\n";

    } catch (const HospitalException& e) {
        cout << "\n  [ERROR] " << e.what() << "\n";
    }
}
