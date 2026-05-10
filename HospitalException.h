
// HospitalException.h - Custom exception hierarchy
// MediCore Hospital Management System

#ifndef HOSPITAL_EXCEPTION_H
#define HOSPITAL_EXCEPTION_H

#include "Utils.h"

// ---------------------------
// Base exception class
// ---------------------------
class HospitalException {
protected:
    char message[200]; // Static array allowed for exception messages
public:
    HospitalException();
    HospitalException(const char* msg);
    virtual ~HospitalException();
    virtual const char* what() const;
};

// ---------------------------
// File not found exception
// ---------------------------
class FileNotFoundException : public HospitalException {
public:
    FileNotFoundException(const char* msg);
};

// ---------------------------
// Insufficient funds exception
// ---------------------------
class InsufficientFundsException : public HospitalException {
public:
    InsufficientFundsException(const char* msg);
};

// ---------------------------
// Invalid input exception
// ---------------------------
class InvalidInputException : public HospitalException {
public:
    InvalidInputException(const char* msg);
};

// ---------------------------
// Slot unavailable exception
// ---------------------------
class SlotUnavailableException : public HospitalException {
public:
    SlotUnavailableException(const char* msg);
};

#endif
