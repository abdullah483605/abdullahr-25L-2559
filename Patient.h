
// Patient.h - Patient class declaration (inherits Person)
// MediCore Hospital Management System

#ifndef PATIENT_H
#define PATIENT_H

#include "Person.h"
#include "Storage.h"

using namespace std;

// Forward declarations
class Doctor;
class Appointment;
class Bill;
class Prescription;
class FileHandler;
class Validator;

class Patient : public Person {
private:
    int age;
    char gender;
    float balance;

public:
    // Constructors and destructor
    Patient();
    Patient(const char* id, const char* name, int age, char gender,
            const char* contact, const char* password, float balance);
    Patient(const Patient& other);
    Patient& operator=(const Patient& other);
    ~Patient();

    // Getters
    int getAge() const;
    char getGender() const;
    float getBalance() const;

    // Setters
    void setAge(int a);
    void setGender(char g);
    void setBalance(float b);

    // Operator overloading
    Patient& operator+=(float amount);  // Add balance
    Patient& operator-=(float amount);  // Deduct balance
    bool operator==(const Patient& other) const; // Compare by ID
    friend ostream& operator<<(ostream& os, const Patient& p);

    // Virtual function implementations
    void display() const override;
    const char* getRole() const override;

    // Patient features
    void bookAppointment(Storage<Doctor>* doctors, Storage<Appointment>* appointments,
                         Storage<Bill>* bills, FileHandler* fh, Validator* val);
    void cancelAppointment(Storage<Appointment>* appointments, Storage<Bill>* bills,
                           Storage<Patient>* patients, FileHandler* fh, Validator* val);
    void viewAppointments(Storage<Appointment>* appointments);
    void viewMedicalRecords(Storage<Prescription>* prescriptions, Storage<Appointment>* appointments);
    void viewBills(Storage<Bill>* bills);
    void payBill(Storage<Bill>* bills, Storage<Patient>* patients, FileHandler* fh, Validator* val);
    void topUpBalance(Storage<Patient>* patients, FileHandler* fh, Validator* val);

    // Login
    bool login(const char* inputID, const char* inputPassword);

    // Convert to CSV
    void toCSV(char* buffer) const;
};

#endif
