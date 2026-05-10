
// Admin.h - Admin class declaration (inherits Person)
// MediCore Hospital Management System

#ifndef ADMIN_H
#define ADMIN_H

#include "Person.h"
#include "Storage.h"

// Forward declarations
class Patient;
class Doctor;
class Appointment;
class Bill;
class Prescription;
class FileHandler;
class Validator;

class Admin : public Person {
public:
    // Constructors and destructor
    Admin();
    Admin(const char* id, const char* name, const char* password);
    Admin(const Admin& other);
    Admin& operator=(const Admin& other);
    ~Admin();

    // Virtual function implementations
    void display() const override;
    const char* getRole() const override;

    // Login
    bool login(const char* inputID, const char* inputPassword);

    // Admin features
    void addDoctor(Storage<Doctor>* doctors, FileHandler* fh, Validator* val);
    void removeDoctor(Storage<Doctor>* doctors, Storage<Appointment>* appointments,
                      FileHandler* fh, Validator* val);
    void viewAllPatients(Storage<Patient>* patients);
    void viewAllDoctors(Storage<Doctor>* doctors);
    void viewAllAppointments(Storage<Appointment>* appointments);
    void viewUnpaidBills(Storage<Bill>* bills);
    void dischargePatient(Storage<Patient>* patients, Storage<Appointment>* appointments,
                          Storage<Bill>* bills, Storage<Prescription>* prescriptions,
                          FileHandler* fh, Validator* val);
    void viewSecurityLog(FileHandler* fh);
    void generateDailyReport(Storage<Appointment>* appointments, Storage<Bill>* bills,
                             Storage<Doctor>* doctors, Validator* val);

    // Convert to CSV
    void toCSV(char* buffer) const;
};

#endif
