
// Validator.h - Input validation class (ONLY validator)
// MediCore Hospital Management System

#ifndef VALIDATOR_H
#define VALIDATOR_H

#include "Utils.h"

class Validator {
public:
    Validator();
    ~Validator();

    // Validate an ID format (non-empty, starts with expected prefix)
    bool validateID(const char* id, const char* prefix) const;

    // Validate date format DD-MM-YYYY and ensure it is current or future
    bool validateDate(const char* date) const;

    // Validate time slot (must be one of the valid slots)
    bool validateTimeSlot(const char* slot) const;

    // Validate contact number (exactly 11 digits)
    bool validateContact(const char* contact) const;

    // Validate password (minimum 6 characters)
    bool validatePassword(const char* password) const;

    // Validate positive float
    bool validatePositiveFloat(float value) const;

    // Validate menu choice within range
    bool validateMenuChoice(int choice, int min, int max) const;

    // Validate non-empty string
    bool validateNonEmpty(const char* input) const;

    // Validate gender (M/F)
    bool validateGender(char gender) const;

    // Validate age (1-150)
    bool validateAge(int age) const;

    // Get current date in DD-MM-YYYY format
    void getCurrentDate(char* buffer) const;

    // Check if a date is today
    bool isToday(const char* date) const;

    // Get list of valid time slots
    static const int NUM_SLOTS = 8;
    void getValidSlots(char slots[][6]) const;
};

#endif
