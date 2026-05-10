
// Appointment.cpp - Appointment class implementation
// MediCore Hospital Management System

#include "../include/Appointment.h"

using namespace std;

// Default constructor
Appointment::Appointment() {
    appointmentID = allocCopy("");
    patientID = allocCopy("");
    doctorID = allocCopy("");
    date = allocCopy("");
    timeSlot = allocCopy("");
    status = allocCopy("Pending");
}

// Parameterized constructor
Appointment::Appointment(const char* aID, const char* pID, const char* dID,
                         const char* dt, const char* slot, const char* stat) {
    appointmentID = allocCopy(aID);
    patientID = allocCopy(pID);
    doctorID = allocCopy(dID);
    date = allocCopy(dt);
    timeSlot = allocCopy(slot);
    status = allocCopy(stat);
}

// Copy constructor
Appointment::Appointment(const Appointment& other) {
    appointmentID = allocCopy(other.appointmentID);
    patientID = allocCopy(other.patientID);
    doctorID = allocCopy(other.doctorID);
    date = allocCopy(other.date);
    timeSlot = allocCopy(other.timeSlot);
    status = allocCopy(other.status);
}

// Assignment operator
Appointment& Appointment::operator=(const Appointment& other) {
    if (this != &other) {
        delete[] appointmentID; delete[] patientID; delete[] doctorID;
        delete[] date; delete[] timeSlot; delete[] status;
        appointmentID = allocCopy(other.appointmentID);
        patientID = allocCopy(other.patientID);
        doctorID = allocCopy(other.doctorID);
        date = allocCopy(other.date);
        timeSlot = allocCopy(other.timeSlot);
        status = allocCopy(other.status);
    }
    return *this;
}

// Destructor
Appointment::~Appointment() {
    delete[] appointmentID; delete[] patientID; delete[] doctorID;
    delete[] date; delete[] timeSlot; delete[] status;
}

// Getters
const char* Appointment::getID() const { return appointmentID; }
const char* Appointment::getPatientID() const { return patientID; }
const char* Appointment::getDoctorID() const { return doctorID; }
const char* Appointment::getDate() const { return date; }
const char* Appointment::getTimeSlot() const { return timeSlot; }
const char* Appointment::getStatus() const { return status; }

// Setters
void Appointment::setStatus(const char* newStatus) {
    delete[] status;
    status = allocCopy(newStatus);
}

// Conflict check: same doctor + same date + same slot + neither cancelled
bool Appointment::operator==(const Appointment& other) const {
    if (strCompare(doctorID, other.doctorID) != 0) return false;
    if (strCompare(date, other.date) != 0) return false;
    if (strCompare(timeSlot, other.timeSlot) != 0) return false;
    // Neither should be cancelled
    if (strCompare(status, "Cancelled") == 0) return false;
    if (strCompare(other.status, "Cancelled") == 0) return false;
    return true;
}

// Formatted display
ostream& operator<<(ostream& os, const Appointment& a) {
    os << "+-----------------------------------------+\n";
    os << "| Appointment ID : " << a.appointmentID << "\n";
    os << "| Patient ID     : " << a.patientID << "\n";
    os << "| Doctor ID      : " << a.doctorID << "\n";
    os << "| Date           : " << a.date << "\n";
    os << "| Time Slot      : " << a.timeSlot << "\n";
    os << "| Status         : " << a.status << "\n";
    os << "+-----------------------------------------+\n";
    return os;
}

// Convert to CSV
void Appointment::toCSV(char* buffer) const {
    strCopy(buffer, appointmentID); strCat(buffer, ",");
    strCat(buffer, patientID); strCat(buffer, ",");
    strCat(buffer, doctorID); strCat(buffer, ",");
    strCat(buffer, date); strCat(buffer, ",");
    strCat(buffer, timeSlot); strCat(buffer, ",");
    strCat(buffer, status);
}
