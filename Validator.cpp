
// Validator.cpp - Input validation implementation
// MediCore Hospital Management System

#include "../include/Validator.h"
#include <ctime>

Validator::Validator() {}
Validator::~Validator() {}

// Validate ID format
bool Validator::validateID(const char* id, const char* prefix) const {
    if (!id || strLen(id) == 0) return false;
    int pLen = strLen(prefix);
    // Check prefix matches
    for (int i = 0; i < pLen; i++) {
        if (*(id + i) == '\0' || *(id + i) != *(prefix + i))
            return false;
    }
    // Rest must be digits
    int i = pLen;
    if (*(id + i) == '\0') return false; // Must have digits after prefix
    while (*(id + i) != '\0') {
        if (*(id + i) < '0' || *(id + i) > '9') return false;
        i++;
    }
    return true;
}

// Validate date DD-MM-YYYY
bool Validator::validateDate(const char* date) const {
    if (!date || strLen(date) != 10) return false;

    // Check format: DD-MM-YYYY
    if (*(date + 2) != '-' || *(date + 5) != '-') return false;

    // Check all other positions are digits
    for (int i = 0; i < 10; i++) {
        if (i == 2 || i == 5) continue;
        if (*(date + i) < '0' || *(date + i) > '9') return false;
    }

    // Extract day, month, year
    int day = (*(date) - '0') * 10 + (*(date + 1) - '0');
    int month = (*(date + 3) - '0') * 10 + (*(date + 4) - '0');
    int year = (*(date + 6) - '0') * 1000 + (*(date + 7) - '0') * 100 +
               (*(date + 8) - '0') * 10 + (*(date + 9) - '0');

    // Basic range validation
    if (month < 1 || month > 12) return false;
    if (day < 1) return false;

    // Days in month
    int daysInMonth = 31;
    if (month == 4 || month == 6 || month == 9 || month == 11) daysInMonth = 30;
    if (month == 2) {
        bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        daysInMonth = isLeap ? 29 : 28;
    }
    if (day > daysInMonth) return false;

    // Must be current year or later
    time_t now = time(nullptr);
    struct tm* current = localtime(&now);
    int curYear = current->tm_year + 1900;
    int curMonth = current->tm_mon + 1;
    int curDay = current->tm_mday;

    if (year < curYear) return false;
    if (year == curYear && month < curMonth) return false;
    if (year == curYear && month == curMonth && day < curDay) return false;

    return true;
}

// Validate time slot
bool Validator::validateTimeSlot(const char* slot) const {
    if (!slot) return false;
    // Valid slots
    char validSlots[8][6];
    getValidSlots(validSlots);
    for (int i = 0; i < NUM_SLOTS; i++) {
        if (strCompare(slot, validSlots[i]) == 0) return true;
    }
    return false;
}

// Validate contact (exactly 11 digits)
bool Validator::validateContact(const char* contact) const {
    if (!contact) return false;
    int len = strLen(contact);
    if (len != 11) return false;
    for (int i = 0; i < len; i++) {
        if (*(contact + i) < '0' || *(contact + i) > '9') return false;
    }
    return true;
}

// Validate password (min 6 characters)
bool Validator::validatePassword(const char* password) const {
    if (!password) return false;
    return strLen(password) >= 6;
}

// Validate positive float
bool Validator::validatePositiveFloat(float value) const {
    return value > 0.0f;
}

// Validate menu choice
bool Validator::validateMenuChoice(int choice, int min, int max) const {
    return choice >= min && choice <= max;
}

// Validate non-empty string
bool Validator::validateNonEmpty(const char* input) const {
    return input && strLen(input) > 0;
}

// Validate gender
bool Validator::validateGender(char gender) const {
    return gender == 'M' || gender == 'F' || gender == 'm' || gender == 'f';
}

// Validate age
bool Validator::validateAge(int age) const {
    return age >= 1 && age <= 150;
}

// Get current date in DD-MM-YYYY
void Validator::getCurrentDate(char* buffer) const {
    time_t now = time(nullptr);
    struct tm* t = localtime(&now);
    // Manual formatting
    int day = t->tm_mday;
    int month = t->tm_mon + 1;
    int year = t->tm_year + 1900;
    *(buffer + 0) = '0' + (day / 10);
    *(buffer + 1) = '0' + (day % 10);
    *(buffer + 2) = '-';
    *(buffer + 3) = '0' + (month / 10);
    *(buffer + 4) = '0' + (month % 10);
    *(buffer + 5) = '-';
    *(buffer + 6) = '0' + (year / 1000);
    *(buffer + 7) = '0' + ((year / 100) % 10);
    *(buffer + 8) = '0' + ((year / 10) % 10);
    *(buffer + 9) = '0' + (year % 10);
    *(buffer + 10) = '\0';
}

// Check if date is today
bool Validator::isToday(const char* date) const {
    char today[11];
    getCurrentDate(today);
    return strCompare(date, today) == 0;
}

// Get valid time slots
void Validator::getValidSlots(char slots[][6]) const {
    strCopy(slots[0], "09:00");
    strCopy(slots[1], "10:00");
    strCopy(slots[2], "11:00");
    strCopy(slots[3], "12:00");
    strCopy(slots[4], "13:00");
    strCopy(slots[5], "14:00");
    strCopy(slots[6], "15:00");
    strCopy(slots[7], "16:00");
}
