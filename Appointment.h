
// Appointment.h - Appointment class declaration
// MediCore Hospital Management System

#ifndef APPOINTMENT_H
#define APPOINTMENT_H

#include <iostream>
#include "Utils.h"

using namespace std;

class Appointment {
private:
    char* appointmentID;
    char* patientID;
    char* doctorID;
    char* date;       // DD-MM-YYYY
    char* timeSlot;   // e.g., "09:00"
    char* status;     // Pending, Completed, Cancelled, No-Show

public:
    // Constructors and destructor
    Appointment();
    Appointment(const char* aID, const char* pID, const char* dID,
                const char* dt, const char* slot, const char* stat);
    Appointment(const Appointment& other);
    Appointment& operator=(const Appointment& other);
    ~Appointment();

    // Getters
    const char* getID() const;
    const char* getPatientID() const;
    const char* getDoctorID() const;
    const char* getDate() const;
    const char* getTimeSlot() const;
    const char* getStatus() const;

    // Setters
    void setStatus(const char* newStatus);

    // Operator overloading: == checks scheduling conflict
    // Same doctor + same date + same slot + neither cancelled
    bool operator==(const Appointment& other) const;

    // Stream output
    friend ostream& operator<<(ostream& os, const Appointment& a);

    // Convert to CSV line for file storage
    void toCSV(char* buffer) const;
};

#endif
