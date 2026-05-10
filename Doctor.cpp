
// Doctor.cpp - Doctor class implementation
// MediCore Hospital Management System

#include "../include/Doctor.h"
#include "../include/Patient.h"
#include "../include/Appointment.h"
#include "../include/Prescription.h"
#include "../include/FileHandler.h"
#include "../include/Validator.h"

using namespace std;
#include "../include/HospitalException.h"

// Default constructor
Doctor::Doctor() : Person(), consultationFee(0.0f) {
    specialization = allocCopy("");
}

// Parameterized constructor
Doctor::Doctor(const char* id, const char* name, const char* spec,
               const char* contact, const char* password, float fee)
    : Person(id, name, contact, password), consultationFee(fee) {
    specialization = allocCopy(spec);
}

// Copy constructor
Doctor::Doctor(const Doctor& other)
    : Person(other), consultationFee(other.consultationFee) {
    specialization = allocCopy(other.specialization);
}

// Assignment operator
Doctor& Doctor::operator=(const Doctor& other) {
    if (this != &other) {
        Person::operator=(other);
        delete[] specialization;
        specialization = allocCopy(other.specialization);
        consultationFee = other.consultationFee;
    }
    return *this;
}

// Destructor
Doctor::~Doctor() {
    delete[] specialization;
}

// Getters
const char* Doctor::getSpecialization() const { return specialization; }
float Doctor::getConsultationFee() const { return consultationFee; }

// Setters
void Doctor::setSpecialization(const char* spec) {
    delete[] specialization;
    specialization = allocCopy(spec);
}
void Doctor::setConsultationFee(float fee) { consultationFee = fee; }

// Operator == (compare by doctor ID)
bool Doctor::operator==(const Doctor& other) const {
    return strCompare(id, other.id) == 0;
}

// Operator << (formatted display)
ostream& operator<<(ostream& os, const Doctor& d) {
    char feeStr[20];
    floatToChar(feeStr, d.consultationFee);
    os << "+===========================================+\n";
    os << "|           DOCTOR INFORMATION             |\n";
    os << "+===========================================+\n";
    os << "| Doctor ID      : " << d.id << "\n";
    os << "| Name           : Dr. " << d.name << "\n";
    os << "| Specialization : " << d.specialization << "\n";
    os << "| Contact        : " << d.contact << "\n";
    os << "| Consultation   : Rs. " << feeStr << "\n";
    os << "+===========================================+\n";
    return os;
}

// Display (virtual)
void Doctor::display() const {
    cout << *this;
}

// Get role (virtual)
const char* Doctor::getRole() const {
    return "Doctor";
}

// Login
bool Doctor::login(const char* inputID, const char* inputPassword) {
    return (strCompare(id, inputID) == 0 && strCompare(password, inputPassword) == 0);
}

// Convert to CSV: doctor_id,name,specialization,contact,password,fee
void Doctor::toCSV(char* buffer) const {
    char feeStr[20];
    floatToChar(feeStr, consultationFee);
    strCopy(buffer, id); strCat(buffer, ",");
    strCat(buffer, name); strCat(buffer, ",");
    strCat(buffer, specialization); strCat(buffer, ",");
    strCat(buffer, contact); strCat(buffer, ",");
    strCat(buffer, password); strCat(buffer, ",");
    strCat(buffer, feeStr);
}


// VIEW TODAY'S APPOINTMENTS (sorted by time ascending)

void Doctor::viewTodayAppointments(Storage<Appointment>* appointments, Validator* val) {
    cout << "\n+===========================================+\n";
    cout << "|       TODAY'S APPOINTMENTS               |\n";
    cout << "+===========================================+\n";

    char today[11];
    val->getCurrentDate(today);
    cout << "  Date: " << today << "\n\n";

    // Collect today's appointments for this doctor
    Appointment* tempArr = new Appointment[100];
    int count = 0;

    for (int i = 0; i < appointments->size(); i++) {
        Appointment* a = appointments->at(i);
        if (strCompare(a->getDoctorID(), id) == 0 &&
            strCompare(a->getDate(), today) == 0) {
            *(tempArr + count) = *a;
            count++;
        }
    }

    if (count == 0) {
        cout << "  No appointments scheduled for today.\n";
        delete[] tempArr;
        return;
    }

    // Bubble sort by time slot ascending
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (strCompare((tempArr + j)->getTimeSlot(), (tempArr + j + 1)->getTimeSlot()) > 0) {
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


// MARK APPOINTMENT COMPLETE

void Doctor::markAppointmentComplete(Storage<Appointment>* appointments, FileHandler* fh, Validator* val) {
    try {
        cout << "\n+===========================================+\n";
        cout << "|     MARK APPOINTMENT COMPLETE            |\n";
        cout << "+===========================================+\n";

        // Show pending appointments for this doctor
        int found = 0;
        for (int i = 0; i < appointments->size(); i++) {
            Appointment* a = appointments->at(i);
            if (strCompare(a->getDoctorID(), id) == 0 &&
                strCompare(a->getStatus(), "Pending") == 0) {
                cout << *a;
                found++;
            }
        }

        if (found == 0) {
            cout << "  No pending appointments.\n";
            return;
        }

        char apptID[30];
        cout << "\n  Enter Appointment ID: ";
        cin.ignore();
        cin.getline(apptID, 30);

        Appointment* appt = appointments->findByID(apptID);
        if (!appt) throw InvalidInputException("Appointment not found.");
        if (strCompare(appt->getDoctorID(), id) != 0)
            throw InvalidInputException("This appointment is not assigned to you.");
        if (strCompare(appt->getStatus(), "Pending") != 0)
            throw InvalidInputException("Only pending appointments can be marked complete.");

        appt->setStatus("Completed");
        fh->rewriteAppointments(appointments);

        cout << "\n  [SUCCESS] Appointment marked as completed.\n";

    } catch (const HospitalException& e) {
        cout << "\n  [ERROR] " << e.what() << "\n";
    }
}


// MARK APPOINTMENT NO-SHOW

void Doctor::markAppointmentNoShow(Storage<Appointment>* appointments, FileHandler* fh, Validator* val) {
    try {
        cout << "\n+===========================================+\n";
        cout << "|     MARK APPOINTMENT NO-SHOW             |\n";
        cout << "+===========================================+\n";

        int found = 0;
        for (int i = 0; i < appointments->size(); i++) {
            Appointment* a = appointments->at(i);
            if (strCompare(a->getDoctorID(), id) == 0 &&
                strCompare(a->getStatus(), "Pending") == 0) {
                cout << *a;
                found++;
            }
        }

        if (found == 0) {
            cout << "  No pending appointments.\n";
            return;
        }

        char apptID[30];
        cout << "\n  Enter Appointment ID: ";
        cin.ignore();
        cin.getline(apptID, 30);

        Appointment* appt = appointments->findByID(apptID);
        if (!appt) throw InvalidInputException("Appointment not found.");
        if (strCompare(appt->getDoctorID(), id) != 0)
            throw InvalidInputException("This appointment is not assigned to you.");
        if (strCompare(appt->getStatus(), "Pending") != 0)
            throw InvalidInputException("Only pending appointments can be marked no-show.");

        appt->setStatus("No-Show");
        fh->rewriteAppointments(appointments);

        cout << "\n  [SUCCESS] Appointment marked as No-Show.\n";

    } catch (const HospitalException& e) {
        cout << "\n  [ERROR] " << e.what() << "\n";
    }
}


// WRITE PRESCRIPTION

void Doctor::writePrescription(Storage<Appointment>* appointments, Storage<Prescription>* prescriptions,
                               FileHandler* fh, Validator* val) {
    try {
        cout << "\n+===========================================+\n";
        cout << "|        WRITE PRESCRIPTION                |\n";
        cout << "+===========================================+\n";

        // Show completed appointments for this doctor
        int found = 0;
        for (int i = 0; i < appointments->size(); i++) {
            Appointment* a = appointments->at(i);
            if (strCompare(a->getDoctorID(), id) == 0 &&
                strCompare(a->getStatus(), "Completed") == 0) {
                cout << *a;
                found++;
            }
        }

        if (found == 0) {
            cout << "  No completed appointments available.\n";
            return;
        }

        char apptID[30];
        cout << "\n  Enter Appointment ID: ";
        cin.ignore();
        cin.getline(apptID, 30);

        Appointment* appt = appointments->findByID(apptID);
        if (!appt) throw InvalidInputException("Appointment not found.");
        if (strCompare(appt->getDoctorID(), id) != 0)
            throw InvalidInputException("This appointment is not assigned to you.");
        if (strCompare(appt->getStatus(), "Completed") != 0)
            throw InvalidInputException("Prescription can only be written for completed appointments.");

        // Check for duplicate prescription
        for (int i = 0; i < prescriptions->size(); i++) {
            if (strCompare(prescriptions->at(i)->getAppointmentID(), apptID) == 0) {
                throw InvalidInputException("Prescription already exists for this appointment.");
            }
        }

        // Get prescription details
        char medicines[500], notes[300];
        cout << "  Enter medicines (max 500 chars): ";
        cin.getline(medicines, 500);

        if (!val->validateNonEmpty(medicines))
            throw InvalidInputException("Medicines field cannot be empty.");

        cout << "  Enter notes (max 300 chars): ";
        cin.getline(notes, 300);

        // Generate prescription ID
        char prescID[20];
        generateNextID(prescID, "PR", prescriptions->size() + 1);

        // Get current date
        char dateStr[11];
        val->getCurrentDate(dateStr);

        // Create prescription
        Prescription newPresc(prescID, apptID, appt->getPatientID(), id, dateStr, medicines, notes);
        prescriptions->add(newPresc);

        // Save to file
        char csvBuf[1024];
        newPresc.toCSV(csvBuf);
        fh->appendRecord("prescriptions.txt", csvBuf);

        cout << "\n  [SUCCESS] Prescription written successfully!\n";
        cout << "  Prescription ID: " << prescID << "\n";

    } catch (const HospitalException& e) {
        cout << "\n  [ERROR] " << e.what() << "\n";
    }
}


// VIEW PATIENT MEDICAL HISTORY (only own patients)

void Doctor::viewPatientMedicalHistory(Storage<Appointment>* appointments, Storage<Prescription>* prescriptions,
                                       Storage<Patient>* patients, Validator* val) {
    try {
        cout << "\n+===========================================+\n";
        cout << "|     PATIENT MEDICAL HISTORY              |\n";
        cout << "+===========================================+\n";

        char patID[30];
        cout << "  Enter Patient ID: ";
        cin.ignore();
        cin.getline(patID, 30);

        Patient* patient = patients->findByID(patID);
        if (!patient) throw InvalidInputException("Patient not found.");

        // Verify this doctor has treated this patient
        bool hasAccess = false;
        for (int i = 0; i < appointments->size(); i++) {
            Appointment* a = appointments->at(i);
            if (strCompare(a->getDoctorID(), id) == 0 &&
                strCompare(a->getPatientID(), patID) == 0) {
                hasAccess = true;
                break;
            }
        }

        if (!hasAccess) {
            throw InvalidInputException("Access denied. You have no appointments with this patient.");
        }

        cout << "\n  Patient: " << patient->getName() << " (" << patID << ")\n";
        cout << "  ------------------------------------------\n";

        int found = 0;
        for (int i = 0; i < prescriptions->size(); i++) {
            Prescription* p = prescriptions->at(i);
            if (strCompare(p->getPatientID(), patID) == 0 &&
                strCompare(p->getDoctorID(), id) == 0) {
                cout << *p;
                found++;
            }
        }

        if (found == 0) {
            cout << "  No prescriptions found for this patient.\n";
        }

    } catch (const HospitalException& e) {
        cout << "\n  [ERROR] " << e.what() << "\n";
    }
}
