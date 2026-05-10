# Abdullah    Roll no : 25L-2559
# MediCore Hospital Management System

A comprehensive console-based Hospital Management System built with C++ using Object-Oriented Programming principles.

## Project Description

MediCore is a fully file-based hospital management system that supports three user roles: **Patient**, **Doctor**, and **Admin**. The system manages appointments, prescriptions, billing, medical records, and security logging. All data persists across program restarts through text file storage.

## Features

### Patient Portal
- Book appointments with doctors (searchable by specialization)
- Cancel appointments with automatic refund
- View appointment history (sorted by date)
- View medical records and prescriptions
- View and pay bills
- Top up account balance
- New patient registration

### Doctor Portal
- View today's appointments (sorted by time)
- Mark appointments as completed or no-show
- Write prescriptions for completed appointments
- View patient medical history (own patients only)

### Admin Portal
- Add and remove doctors
- View all patients, doctors, and appointments
- View unpaid bills
- Discharge patients (with archiving)
- View security log
- Generate daily reports with doctor-wise summary

### Security
- Password-based authentication
- 3-attempt login lockout
- All login attempts logged with timestamps

## OOP Concepts Used

| Concept | Implementation |
|---------|---------------|
| **Inheritance** | Patient, Doctor, Admin inherit from Person |
| **Polymorphism** | Virtual functions display() and getRole() |
| **Abstraction** | Person is an abstract base class |
| **Templates** | Storage\<T\> generic container class |
| **Operator Overloading** | +=, -=, ==, << operators |
| **Exception Handling** | Custom exception hierarchy |
| **Encapsulation** | Private data with public interfaces |

## Folder Structure

```
MediCore/
├── main.cpp                  # Entry point
├── README.md                 # This file
│
├── include/                  # Header files
│   ├── Utils.h               # Manual string utilities
│   ├── Person.h              # Abstract base class
│   ├── Patient.h             # Patient class
│   ├── Doctor.h              # Doctor class
│   ├── Admin.h               # Admin class
│   ├── Appointment.h         # Appointment class
│   ├── Bill.h                # Bill class
│   ├── Prescription.h        # Prescription class
│   ├── Storage.h             # Template storage class
│   ├── FileHandler.h         # File I/O handler
│   ├── Validator.h           # Input validator
│   └── HospitalException.h   # Exception classes
│
├── src/                      # Source files
│   ├── Person.cpp
│   ├── Patient.cpp
│   ├── Doctor.cpp
│   ├── Admin.cpp
│   ├── Appointment.cpp
│   ├── Bill.cpp
│   ├── Prescription.cpp
│   ├── FileHandler.cpp
│   ├── Validator.cpp
│   └── HospitalException.cpp
│
└── data/                     # Data files (auto-created)
    ├── patients.txt
    ├── doctors.txt
    ├── admin.txt
    ├── appointments.txt
    ├── prescriptions.txt
    ├── bills.txt
    ├── security_log.txt
    └── discharged.txt
```

## How to Compile

### Using g++ (MinGW on Windows / GCC on Linux)

```bash
cd MediCore
g++ -o hospital main.cpp src/Person.cpp src/Patient.cpp src/Doctor.cpp src/Admin.cpp src/Appointment.cpp src/Bill.cpp src/Prescription.cpp src/FileHandler.cpp src/Validator.cpp src/HospitalException.cpp
```

### Alternative (compile all .cpp files)

```bash
g++ -o hospital main.cpp src/*.cpp
```

## How to Run

```bash
# Windows
hospital.exe

# Linux/Mac
./hospital
```

## Sample Login Credentials

### Patients
| ID   | Password  |
|------|-----------|
| P001 | ahmed123  |
| P002 | sara1234  |
| P003 | bilal456  |

### Doctors
| ID   | Password  |
|------|-----------|
| D001 | ayesha12  |
| D002 | usman123  |
| D003 | fatima12  |

### Admin
| ID     | Password  |
|--------|-----------|
| ADM001 | admin123  |

## File Formats

| File | Format |
|------|--------|
| patients.txt | patient_id,name,age,gender,contact,password,balance |
| doctors.txt | doctor_id,name,specialization,contact,password,fee |
| admin.txt | admin_id,name,password |
| appointments.txt | appointment_id,patient_id,doctor_id,date,time_slot,status |
| prescriptions.txt | prescription_id,appointment_id,patient_id,doctor_id,date,medicines,notes |
| bills.txt | bill_id,patient_id,appointment_id,amount,status,date |
| security_log.txt | timestamp,role,entered_id,result |

## Technical Constraints

- No `std::vector` or `std::string` used
- No `strcmp()`, `strtok()` or similar built-in string helpers
- Only `char` arrays for strings
- Dynamic memory allocation throughout
- Manual sorting algorithms (Bubble Sort)
- Manual input validation
- Template-based generic storage

## GitHub Repository

> https://github.com/your-username/MediCore-Hospital-Management-System

---

**Developed for OOP Course Project | C++ | 2026**
