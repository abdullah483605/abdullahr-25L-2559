
// Bill.cpp - Bill class implementation
// MediCore Hospital Management System

#include "../include/Bill.h"

using namespace std;

// Default constructor
Bill::Bill() : amount(0.0f) {
    billID = allocCopy("");
    patientID = allocCopy("");
    appointmentID = allocCopy("");
    status = allocCopy("Unpaid");
    date = allocCopy("");
}

// Parameterized constructor
Bill::Bill(const char* bID, const char* pID, const char* aID,
           float amt, const char* stat, const char* dt)
    : amount(amt) {
    billID = allocCopy(bID);
    patientID = allocCopy(pID);
    appointmentID = allocCopy(aID);
    status = allocCopy(stat);
    date = allocCopy(dt);
}

// Copy constructor
Bill::Bill(const Bill& other) : amount(other.amount) {
    billID = allocCopy(other.billID);
    patientID = allocCopy(other.patientID);
    appointmentID = allocCopy(other.appointmentID);
    status = allocCopy(other.status);
    date = allocCopy(other.date);
}

// Assignment operator
Bill& Bill::operator=(const Bill& other) {
    if (this != &other) {
        delete[] billID; delete[] patientID; delete[] appointmentID;
        delete[] status; delete[] date;
        billID = allocCopy(other.billID);
        patientID = allocCopy(other.patientID);
        appointmentID = allocCopy(other.appointmentID);
        amount = other.amount;
        status = allocCopy(other.status);
        date = allocCopy(other.date);
    }
    return *this;
}

// Destructor
Bill::~Bill() {
    delete[] billID; delete[] patientID; delete[] appointmentID;
    delete[] status; delete[] date;
}

// Getters
const char* Bill::getID() const { return billID; }
const char* Bill::getPatientID() const { return patientID; }
const char* Bill::getAppointmentID() const { return appointmentID; }
float Bill::getAmount() const { return amount; }
const char* Bill::getStatus() const { return status; }
const char* Bill::getDate() const { return date; }

// Setters
void Bill::setStatus(const char* newStatus) {
    delete[] status;
    status = allocCopy(newStatus);
}

// Formatted display
ostream& operator<<(ostream& os, const Bill& b) {
    char amtStr[20];
    floatToChar(amtStr, b.amount);
    os << "+-----------------------------------------+\n";
    os << "| Bill ID        : " << b.billID << "\n";
    os << "| Patient ID     : " << b.patientID << "\n";
    os << "| Appointment ID : " << b.appointmentID << "\n";
    os << "| Amount         : Rs. " << amtStr << "\n";
    os << "| Status         : " << b.status << "\n";
    os << "| Date           : " << b.date << "\n";
    os << "+-----------------------------------------+\n";
    return os;
}

// Convert to CSV
void Bill::toCSV(char* buffer) const {
    char amtStr[20];
    floatToChar(amtStr, amount);
    strCopy(buffer, billID); strCat(buffer, ",");
    strCat(buffer, patientID); strCat(buffer, ",");
    strCat(buffer, appointmentID); strCat(buffer, ",");
    strCat(buffer, amtStr); strCat(buffer, ",");
    strCat(buffer, status); strCat(buffer, ",");
    strCat(buffer, date);
}
