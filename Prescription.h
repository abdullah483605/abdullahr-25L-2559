
// Prescription.h - Prescription class declaration
// MediCore Hospital Management System

#ifndef PRESCRIPTION_H
#define PRESCRIPTION_H

#include <iostream>
#include "Utils.h"

using namespace std;

class Prescription {
private:
    char* prescriptionID;
    char* appointmentID;
    char* patientID;
    char* doctorID;
    char* date;
    char* medicines;  // Up to 500 chars
    char* notes;      // Up to 300 chars

public:
    Prescription();
    Prescription(const char* prID, const char* aID, const char* pID,
                 const char* dID, const char* dt, const char* meds, const char* nt);
    Prescription(const Prescription& other);
    Prescription& operator=(const Prescription& other);
    ~Prescription();

    // Getters
    const char* getID() const;
    const char* getAppointmentID() const;
    const char* getPatientID() const;
    const char* getDoctorID() const;
    const char* getDate() const;
    const char* getMedicines() const;
    const char* getNotes() const;

    // Formatted display
    friend ostream& operator<<(ostream& os, const Prescription& p);

    // Convert to CSV
    void toCSV(char* buffer) const;
};

#endif
