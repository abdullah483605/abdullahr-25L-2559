
// Bill.h - Bill class declaration
// MediCore Hospital Management System

#ifndef BILL_H
#define BILL_H

#include <iostream>
#include "Utils.h"

using namespace std;

class Bill {
private:
    char* billID;
    char* patientID;
    char* appointmentID;
    float amount;
    char* status;     // Unpaid, Paid, Refunded
    char* date;       // DD-MM-YYYY

public:
    Bill();
    Bill(const char* bID, const char* pID, const char* aID,
         float amt, const char* stat, const char* dt);
    Bill(const Bill& other);
    Bill& operator=(const Bill& other);
    ~Bill();

    // Getters
    const char* getID() const;
    const char* getPatientID() const;
    const char* getAppointmentID() const;
    float getAmount() const;
    const char* getStatus() const;
    const char* getDate() const;

    // Setters
    void setStatus(const char* newStatus);

    // Formatted display
    friend ostream& operator<<(ostream& os, const Bill& b);

    // Convert to CSV
    void toCSV(char* buffer) const;
};

#endif
