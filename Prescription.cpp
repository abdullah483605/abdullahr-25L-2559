
// Prescription.cpp - Prescription class implementation
// MediCore Hospital Management System

#include "../include/Prescription.h"

using namespace std;

// Default constructor
Prescription::Prescription() {
    prescriptionID = allocCopy("");
    appointmentID = allocCopy("");
    patientID = allocCopy("");
    doctorID = allocCopy("");
    date = allocCopy("");
    medicines = allocCopy("");
    notes = allocCopy("");
}

// Parameterized constructor
Prescription::Prescription(const char* prID, const char* aID, const char* pID,
                           const char* dID, const char* dt, const char* meds, const char* nt) {
    prescriptionID = allocCopy(prID);
    appointmentID = allocCopy(aID);
    patientID = allocCopy(pID);
    doctorID = allocCopy(dID);
    date = allocCopy(dt);
    medicines = allocCopy(meds);
    notes = allocCopy(nt);
}

// Copy constructor
Prescription::Prescription(const Prescription& other) {
    prescriptionID = allocCopy(other.prescriptionID);
    appointmentID = allocCopy(other.appointmentID);
    patientID = allocCopy(other.patientID);
    doctorID = allocCopy(other.doctorID);
    date = allocCopy(other.date);
    medicines = allocCopy(other.medicines);
    notes = allocCopy(other.notes);
}

// Assignment operator
Prescription& Prescription::operator=(const Prescription& other) {
    if (this != &other) {
        delete[] prescriptionID; delete[] appointmentID; delete[] patientID;
        delete[] doctorID; delete[] date; delete[] medicines; delete[] notes;
        prescriptionID = allocCopy(other.prescriptionID);
        appointmentID = allocCopy(other.appointmentID);
        patientID = allocCopy(other.patientID);
        doctorID = allocCopy(other.doctorID);
        date = allocCopy(other.date);
        medicines = allocCopy(other.medicines);
        notes = allocCopy(other.notes);
    }
    return *this;
}

// Destructor
Prescription::~Prescription() {
    delete[] prescriptionID; delete[] appointmentID; delete[] patientID;
    delete[] doctorID; delete[] date; delete[] medicines; delete[] notes;
}

// Getters
const char* Prescription::getID() const { return prescriptionID; }
const char* Prescription::getAppointmentID() const { return appointmentID; }
const char* Prescription::getPatientID() const { return patientID; }
const char* Prescription::getDoctorID() const { return doctorID; }
const char* Prescription::getDate() const { return date; }
const char* Prescription::getMedicines() const { return medicines; }
const char* Prescription::getNotes() const { return notes; }

// Formatted display
ostream& operator<<(ostream& os, const Prescription& p) {
    os << "+==============================================+\n";
    os << "|           PRESCRIPTION DETAILS               |\n";
    os << "+==============================================+\n";
    os << "| Prescription ID : " << p.prescriptionID << "\n";
    os << "| Appointment ID  : " << p.appointmentID << "\n";
    os << "| Patient ID      : " << p.patientID << "\n";
    os << "| Doctor ID       : " << p.doctorID << "\n";
    os << "| Date            : " << p.date << "\n";
    os << "| Medicines       : " << p.medicines << "\n";
    os << "| Notes           : " << p.notes << "\n";
    os << "+==============================================+\n";
    return os;
}

// Convert to CSV (using | as delimiter for medicines/notes that may contain commas)
void Prescription::toCSV(char* buffer) const {
    strCopy(buffer, prescriptionID); strCat(buffer, ",");
    strCat(buffer, appointmentID); strCat(buffer, ",");
    strCat(buffer, patientID); strCat(buffer, ",");
    strCat(buffer, doctorID); strCat(buffer, ",");
    strCat(buffer, date); strCat(buffer, ",");
    strCat(buffer, medicines); strCat(buffer, ",");
    strCat(buffer, notes);
}
