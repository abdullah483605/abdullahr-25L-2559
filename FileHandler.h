
// FileHandler.h - File I/O handler (ONLY class for file ops)
// MediCore Hospital Management System

#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include "Storage.h"
#include "HospitalException.h"

// Forward declarations
class Patient;
class Doctor;
class Admin;
class Appointment;
class Bill;
class Prescription;

class FileHandler {
private:
    char dataPath[256]; // Path to data directory

    // Helper: build full file path
    void buildPath(char* dest, const char* filename) const;

public:
    FileHandler();
    FileHandler(const char* path);
    ~FileHandler();

    // Load functions
    void loadPatients(Storage<Patient>* storage);
    void loadDoctors(Storage<Doctor>* storage);
    void loadAdmins(Storage<Admin>* storage);
    void loadAppointments(Storage<Appointment>* storage);
    void loadBills(Storage<Bill>* storage);
    void loadPrescriptions(Storage<Prescription>* storage);

    // Append a single record line to a file
    void appendRecord(const char* filename, const char* csvLine);

    // Update a record by searching for matching ID in first CSV field
    void updateRecordByID(const char* filename, const char* id, const char* newCSVLine);

    // Delete a record by ID (first CSV field)
    void deleteRecordByID(const char* filename, const char* id);

    // Rewrite entire file from storage
    void rewritePatients(Storage<Patient>* storage);
    void rewriteDoctors(Storage<Doctor>* storage);
    void rewriteAppointments(Storage<Appointment>* storage);
    void rewriteBills(Storage<Bill>* storage);
    void rewritePrescriptions(Storage<Prescription>* storage);

    // Security log
    void logSecurity(const char* role, const char* enteredID, const char* result);

    // Append to discharged file
    void appendDischarged(const char* record);

    // Read security log and print
    void readSecurityLog() const;

    // Getter
    const char* getDataPath() const;
};

#endif
