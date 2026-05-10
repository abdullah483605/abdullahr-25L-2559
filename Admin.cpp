// 
// Admin.cpp - Admin class implementation
// MediCore Hospital Management System
// 
#include "../include/Admin.h"

using namespace std;
#include "../include/Patient.h"
#include "../include/Doctor.h"
#include "../include/Appointment.h"
#include "../include/Bill.h"
#include "../include/Prescription.h"
#include "../include/FileHandler.h"
#include "../include/Validator.h"
#include "../include/HospitalException.h"

// Default constructor
Admin::Admin() : Person() {}

// Parameterized constructor (admin has no contact in file)
Admin::Admin(const char* id, const char* name, const char* password)
    : Person(id, name, "N/A", password) {}

// Copy constructor
Admin::Admin(const Admin& other) : Person(other) {}

// Assignment operator
Admin& Admin::operator=(const Admin& other) {
    if (this != &other) {
        Person::operator=(other);
    }
    return *this;
}

// Destructor
Admin::~Admin() {}

// Display (virtual)
void Admin::display() const {
    cout << "+===========================================+\n";
    cout << "|           ADMIN INFORMATION              |\n";
    cout << "+===========================================+\n";
    cout << "| Admin ID : " << id << "\n";
    cout << "| Name     : " << name << "\n";
    cout << "+===========================================+\n";
}

// Get role (virtual)
const char* Admin::getRole() const {
    return "Admin";
}

// Login
bool Admin::login(const char* inputID, const char* inputPassword) {
    return (strCompare(id, inputID) == 0 && strCompare(password, inputPassword) == 0);
}

// Convert to CSV: admin_id,name,password
void Admin::toCSV(char* buffer) const {
    strCopy(buffer, id); strCat(buffer, ",");
    strCat(buffer, name); strCat(buffer, ",");
    strCat(buffer, password);
}

// 
// ADD DOCTOR
//
void Admin::addDoctor(Storage<Doctor>* doctors, FileHandler* fh, Validator* val) {
    try {
        cout << "\n+===========================================+\n";
        cout << "|            ADD NEW DOCTOR                |\n";
        cout << "+===========================================+\n";

        char docName[100], spec[100], cont[20], pass[50], feeStr[20];

        cout << "  Enter Doctor Name: ";
        cin.ignore();
        cin.getline(docName, 100);
        if (!val->validateNonEmpty(docName))
            throw InvalidInputException("Name cannot be empty.");

        cout << "  Enter Specialization: ";
        cin.getline(spec, 100);
        if (!val->validateNonEmpty(spec))
            throw InvalidInputException("Specialization cannot be empty.");

        cout << "  Enter Contact (11 digits): ";
        cin.getline(cont, 20);
        if (!val->validateContact(cont))
            throw InvalidInputException("Contact must be exactly 11 digits.");

        cout << "  Enter Password (min 6 chars): ";
        cin.getline(pass, 50);
        if (!val->validatePassword(pass))
            throw InvalidInputException("Password must be at least 6 characters.");

        cout << "  Enter Consultation Fee: Rs. ";
        cin.getline(feeStr, 20);
        float fee = charToFloat(feeStr);
        if (!val->validatePositiveFloat(fee))
            throw InvalidInputException("Fee must be a positive number.");

        // Generate doctor ID
        char docID[20];
        generateNextID(docID, "D", doctors->size() + 1);

        // Create and add doctor
        Doctor newDoc(docID, docName, spec, cont, pass, fee);
        doctors->add(newDoc);

        // Save to file
        char csvBuf[512];
        newDoc.toCSV(csvBuf);
        fh->appendRecord("doctors.txt", csvBuf);

        cout << "\n  [SUCCESS] Doctor added successfully!\n";
        cout << "  Doctor ID: " << docID << "\n";

    } catch (const HospitalException& e) {
        cout << "\n  [ERROR] " << e.what() << "\n";
    }
}

// 
// REMOVE DOCTOR
// 
void Admin::removeDoctor(Storage<Doctor>* doctors, Storage<Appointment>* appointments,
                         FileHandler* fh, Validator* val) {
    try {
        cout << "\n+===========================================+\n";
        cout << "|          REMOVE DOCTOR                   |\n";
        cout << "+===========================================+\n";

        // Display all doctors
        if (doctors->size() == 0) {
            cout << "  No doctors in the system.\n";
            return;
        }

        for (int i = 0; i < doctors->size(); i++) {
            cout << *(doctors->at(i));
        }

        char docID[30];
        cout << "\n  Enter Doctor ID to remove: ";
        cin.ignore();
        cin.getline(docID, 30);

        Doctor* doc = doctors->findByID(docID);
        if (!doc) throw InvalidInputException("Doctor not found.");

        // Check for pending appointments
        for (int i = 0; i < appointments->size(); i++) {
            Appointment* a = appointments->at(i);
            if (strCompare(a->getDoctorID(), docID) == 0 &&
                strCompare(a->getStatus(), "Pending") == 0) {
                throw InvalidInputException("Cannot remove doctor with pending appointments.");
            }
        }

        // Confirm removal
        char confirm[5];
        cout << "  Confirm removal of Dr. " << doc->getName() << "? (y/n): ";
        cin.getline(confirm, 5);
        if (*confirm != 'y' && *confirm != 'Y') {
            cout << "  Removal cancelled.\n";
            return;
        }

        doctors->removeByID(docID);
        fh->rewriteDoctors(doctors);

        cout << "\n  [SUCCESS] Doctor removed successfully.\n";

    } catch (const HospitalException& e) {
        cout << "\n  [ERROR] " << e.what() << "\n";
    }
}

// 
// VIEW ALL PATIENTS
// 
void Admin::viewAllPatients(Storage<Patient>* patients) {
    cout << "\n+===========================================+\n";
    cout << "|          ALL PATIENTS                    |\n";
    cout << "+===========================================+\n";

    if (patients->size() == 0) {
        cout << "  No patients registered.\n";
        return;
    }

    for (int i = 0; i < patients->size(); i++) {
        cout << *(patients->at(i));
    }
    cout << "  Total Patients: " << patients->size() << "\n";
}

// 
// VIEW ALL DOCTORS
// 
void Admin::viewAllDoctors(Storage<Doctor>* doctors) {
    cout << "\n+===========================================+\n";
    cout << "|          ALL DOCTORS                     |\n";
    cout << "+===========================================+\n";

    if (doctors->size() == 0) {
        cout << "  No doctors registered.\n";
        return;
    }

    for (int i = 0; i < doctors->size(); i++) {
        cout << *(doctors->at(i));
    }
    cout << "  Total Doctors: " << doctors->size() << "\n";
}

// 
// VIEW ALL APPOINTMENTS
// 
void Admin::viewAllAppointments(Storage<Appointment>* appointments) {
    cout << "\n+===========================================+\n";
    cout << "|         ALL APPOINTMENTS                 |\n";
    cout << "+===========================================+\n";

    if (appointments->size() == 0) {
        cout << "  No appointments found.\n";
        return;
    }

    for (int i = 0; i < appointments->size(); i++) {
        cout << *(appointments->at(i));
    }
    cout << "  Total Appointments: " << appointments->size() << "\n";
}

// 
// VIEW UNPAID BILLS
// 
void Admin::viewUnpaidBills(Storage<Bill>* bills) {
    cout << "\n+===========================================+\n";
    cout << "|          UNPAID BILLS                    |\n";
    cout << "+===========================================+\n";

    float total = 0.0f;
    int count = 0;
    for (int i = 0; i < bills->size(); i++) {
        Bill* b = bills->at(i);
        if (strCompare(b->getStatus(), "Unpaid") == 0) {
            cout << *b;
            total += b->getAmount();
            count++;
        }
    }

    if (count == 0) {
        std::cout << "  No unpaid bills.\n";
        return;
    }

    char totalStr[20];
    floatToChar(totalStr, total);
    std::cout << "\n  Total Unpaid Bills: " << count << "\n";
    std::cout << "  Total Outstanding: Rs. " << totalStr << "\n";
}

// 
// DISCHARGE PATIENT
// 
void Admin::dischargePatient(Storage<Patient>* patients, Storage<Appointment>* appointments,
                             Storage<Bill>* bills, Storage<Prescription>* prescriptions,
                             FileHandler* fh, Validator* val) {
    try {
        std::cout << "\n+===========================================+\n";
        std::cout << "|        DISCHARGE PATIENT                 |\n";
        std::cout << "+===========================================+\n";

        char patID[30];
        std::cout << "  Enter Patient ID: ";
        std::cin.ignore();
        std::cin.getline(patID, 30);

        Patient* patient = patients->findByID(patID);
        if (!patient) throw InvalidInputException("Patient not found.");

        // Check for unpaid bills
        for (int i = 0; i < bills->size(); i++) {
            Bill* b = bills->at(i);
            if (strCompare(b->getPatientID(), patID) == 0 &&
                strCompare(b->getStatus(), "Unpaid") == 0) {
                throw InvalidInputException("Patient has unpaid bills. Cannot discharge.");
            }
        }

        // Check for pending appointments
        for (int i = 0; i < appointments->size(); i++) {
            Appointment* a = appointments->at(i);
            if (strCompare(a->getPatientID(), patID) == 0 &&
                strCompare(a->getStatus(), "Pending") == 0) {
                throw InvalidInputException("Patient has pending appointments. Cannot discharge.");
            }
        }

        // Archive patient record
        char csvBuf[512];
        patient->toCSV(csvBuf);
        fh->appendDischarged(csvBuf);

        // Confirm
        char confirm[5];
        std::cout << "  Confirm discharge of " << patient->getName() << "? (y/n): ";
        std::cin.getline(confirm, 5);
        if (*confirm != 'y' && *confirm != 'Y') {
            std::cout << "  Discharge cancelled.\n";
            return;
        }

        // Remove patient
        patients->removeByID(patID);
        fh->rewritePatients(patients);

        std::cout << "\n  [SUCCESS] Patient discharged and archived.\n";

    } catch (const HospitalException& e) {
        std::cout << "\n  [ERROR] " << e.what() << "\n";
    }
}

// 
// VIEW SECURITY LOG
// 
void Admin::viewSecurityLog(FileHandler* fh) {
    fh->readSecurityLog();
}

// 
// GENERATE DAILY REPORT
// 
void Admin::generateDailyReport(Storage<Appointment>* appointments, Storage<Bill>* bills,
                                Storage<Doctor>* doctors, Validator* val) {
    char today[11];
    val->getCurrentDate(today);

    std::cout << "\n+==============================================================+\n";
    std::cout << "|              DAILY REPORT - " << today << "                  |\n";
    std::cout << "+==============================================================+\n";

    int totalAppt = 0, completed = 0, pending = 0, cancelled = 0, noShow = 0;
    float revenue = 0.0f, outstanding = 0.0f;

    // Count appointment statuses for today
    for (int i = 0; i < appointments->size(); i++) {
        Appointment* a = appointments->at(i);
        if (strCompare(a->getDate(), today) == 0) {
            totalAppt++;
            if (strCompare(a->getStatus(), "Completed") == 0) completed++;
            else if (strCompare(a->getStatus(), "Pending") == 0) pending++;
            else if (strCompare(a->getStatus(), "Cancelled") == 0) cancelled++;
            else if (strCompare(a->getStatus(), "No-Show") == 0) noShow++;
        }
    }

    // Calculate revenue and outstanding
    for (int i = 0; i < bills->size(); i++) {
        Bill* b = bills->at(i);
        if (strCompare(b->getDate(), today) == 0) {
            if (strCompare(b->getStatus(), "Paid") == 0)
                revenue += b->getAmount();
            else if (strCompare(b->getStatus(), "Unpaid") == 0)
                outstanding += b->getAmount();
        }
    }

    char revStr[20], outStr[20];
    floatToChar(revStr, revenue);
    floatToChar(outStr, outstanding);

    std::cout << "|\n";
    std::cout << "|  Total Appointments  : " << totalAppt << "\n";
    std::cout << "|  Completed           : " << completed << "\n";
    std::cout << "|  Pending             : " << pending << "\n";
    std::cout << "|  Cancelled           : " << cancelled << "\n";
    std::cout << "|  No-Show             : " << noShow << "\n";
    std::cout << "|  Revenue Collected   : Rs. " << revStr << "\n";
    std::cout << "|  Outstanding Bills   : Rs. " << outStr << "\n";
    std::cout << "|\n";

    // Doctor-wise summary
    std::cout << "|  --- DOCTOR-WISE SUMMARY ---\n";
    std::cout << "|  Doctor ID  | Name            | Appts | Completed\n";
    std::cout << "|  -----------------------------------------------\n";

    for (int d = 0; d < doctors->size(); d++) {
        Doctor* doc = doctors->at(d);
        int docTotal = 0, docCompleted = 0;
        for (int i = 0; i < appointments->size(); i++) {
            Appointment* a = appointments->at(i);
            if (strCompare(a->getDoctorID(), doc->getID()) == 0 &&
                strCompare(a->getDate(), today) == 0) {
                docTotal++;
                if (strCompare(a->getStatus(), "Completed") == 0)
                    docCompleted++;
            }
        }
        if (docTotal > 0) {
            std::cout << "|  " << doc->getID() << "     | Dr. " << doc->getName()
                      << " | " << docTotal << "     | " << docCompleted << "\n";
        }
    }

    std::cout << "+==============================================================+\n";
}
