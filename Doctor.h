
// Doctor.h - Doctor class declaration (inherits Person)
// MediCore Hospital Management System

#ifndef DOCTOR_H
#define DOCTOR_H

#include "Person.h"
#include "Storage.h"

using namespace std;

// Forward declarations
class Patient;
class Appointment;
class Prescription;
class FileHandler;
class Validator;

class Doctor : public Person {
private:
    char* specialization;
    float consultationFee;

public:
    // Constructors and destructor
    Doctor();
    Doctor(const char* id, const char* name, const char* spec,
           const char* contact, const char* password, float fee);
    Doctor(const Doctor& other);
    Doctor& operator=(const Doctor& other);
    ~Doctor();

    // Getters
    const char* getSpecialization() const;
    float getConsultationFee() const;

    // Setters
    void setSpecialization(const char* spec);
    void setConsultationFee(float fee);

    // Operator overloading
    bool operator==(const Doctor& other) const;  // Compare by ID
    friend ostream& operator<<(ostream& os, const Doctor& d);

    // Virtual function implementations
    void display() const override;
    const char* getRole() const override;

    // Doctor features
    void viewTodayAppointments(Storage<Appointment>* appointments, Validator* val);
    void markAppointmentComplete(Storage<Appointment>* appointments, FileHandler* fh, Validator* val);
    void markAppointmentNoShow(Storage<Appointment>* appointments, FileHandler* fh, Validator* val);
    void writePrescription(Storage<Appointment>* appointments, Storage<Prescription>* prescriptions,
                           FileHandler* fh, Validator* val);
    void viewPatientMedicalHistory(Storage<Appointment>* appointments, Storage<Prescription>* prescriptions,
                                   Storage<Patient>* patients, Validator* val);

    // Login
    bool login(const char* inputID, const char* inputPassword);

    // Convert to CSV
    void toCSV(char* buffer) const;
};

#endif
