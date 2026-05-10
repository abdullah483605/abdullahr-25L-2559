
// HospitalException.cpp - Exception class implementations
// MediCore Hospital Management System

#include "../include/HospitalException.h"

//  HospitalException (Base) 
HospitalException::HospitalException() {
    strCopy(message, "An error occurred in the hospital system.");
}

HospitalException::HospitalException(const char* msg) {
    strCopy(message, msg);
}

HospitalException::~HospitalException() {}

const char* HospitalException::what() const {
    return message;
}

//  FileNotFoundException 
FileNotFoundException::FileNotFoundException(const char* msg)
    : HospitalException(msg) {}

//  InsufficientFundsException 
InsufficientFundsException::InsufficientFundsException(const char* msg)
    : HospitalException(msg) {}

//  InvalidInputException 
InvalidInputException::InvalidInputException(const char* msg)
    : HospitalException(msg) {}

//  SlotUnavailableException 
SlotUnavailableException::SlotUnavailableException(const char* msg)
    : HospitalException(msg) {}
