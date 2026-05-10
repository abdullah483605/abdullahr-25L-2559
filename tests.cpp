#include <iostream>
#include "include/Utils.h"
#include "include/Validator.h"
#include "include/Storage.h"
#include "include/Patient.h"
#include "include/Doctor.h"
#include "include/Admin.h"
#include "include/Appointment.h"
#include "include/Bill.h"
#include "include/Prescription.h"
#include "include/FileHandler.h"
#include "include/HospitalException.h"

using namespace std;

// Simple test framework macro
int testsRun = 0;
int testsPassed = 0;

#define ASSERT(condition, message) \
    do { \
        testsRun++; \
        if (!(condition)) { \
            cout << "[FAILED] " << message << " (Line " << __LINE__ << ")\n"; \
        } else { \
            cout << "[PASSED] " << message << "\n"; \
            testsPassed++; \
        } \
    } while (0)

void testUtils() {
    cout << "\n--- Testing Utils ---\n";
    ASSERT(strLen("hello") == 5, "strLen");
    
    char dest[20];
    strCopy(dest, "test");
    ASSERT(strCompare(dest, "test") == 0, "strCopy and strCompare");
    ASSERT(strCompareCI("Test", "test") == 0, "strCompareCI");
    
    char numStr[20];
    intToChar(numStr, 123);
    ASSERT(strCompare(numStr, "123") == 0, "intToChar");
    ASSERT(charToInt("123") == 123, "charToInt");
    
    char nextId[20];
    generateNextID(nextId, "P", 5);
    ASSERT(strCompare(nextId, "P005") == 0, "generateNextID < 10");
    generateNextID(nextId, "P", 15);
    ASSERT(strCompare(nextId, "P015") == 0, "generateNextID < 100");
}

void testValidator() {
    cout << "\n--- Testing Validator ---\n";
    Validator v;
    ASSERT(v.validateID("P001", "P"), "Valid ID format");
    ASSERT(!v.validateID("001", "P"), "Invalid ID format without prefix");
    
    // Date validation
    ASSERT(!v.validateDate("10-10-2020"), "Past date should fail");
    ASSERT(!v.validateDate("invalid"), "Invalid date string");
    
    ASSERT(v.validateContact("03001234567"), "Valid contact exactly 11 digits");
    ASSERT(!v.validateContact("0300123456"), "Invalid contact < 11 digits");
    
    ASSERT(v.validatePassword("secret"), "Valid password length >= 6");
    ASSERT(!v.validatePassword("sec"), "Invalid password length < 6");
}

void testStorage() {
    cout << "\n--- Testing Storage ---\n";
    Storage<Patient> storage;
    ASSERT(storage.size() == 0, "Storage initially empty");
    
    Patient p1("P001", "Ali", 30, 'M', "03001234567", "pass123", 100.0f);
    storage.add(p1);
    ASSERT(storage.size() == 1, "Storage size increases after add");
    
    Patient* found = storage.findByID("P001");
    ASSERT(found != nullptr, "Find existing element returns pointer");
    ASSERT(strCompare(found->getName(), "Ali") == 0, "Found element data matches");
    
    storage.removeByID("P001");
    ASSERT(storage.size() == 0, "Storage size decreases after remove");
}

void testModels() {
    cout << "\n--- Testing Models ---\n";
    Patient p("P001", "Bob", 25, 'M', "03111234567", "pass123", 500.0f);
    ASSERT(p.getAge() == 25, "Patient age getter works");
    ASSERT(p.getBalance() == 500.0f, "Patient balance getter works");
    
    p += 100.0f;
    ASSERT(p.getBalance() == 600.0f, "Patient operator += works");
    p -= 50.0f;
    ASSERT(p.getBalance() == 550.0f, "Patient operator -= works");
    
    ASSERT(p.login("P001", "pass123"), "Patient valid login succeeds");
    ASSERT(!p.login("P001", "wrong"), "Patient invalid login fails");
    
    Doctor d("D001", "Dr. Smith", "Cardiology", "03221234567", "docpass", 1500.0f);
    ASSERT(strCompare(d.getSpecialization(), "Cardiology") == 0, "Doctor specialization getter works");
    ASSERT(d.getConsultationFee() == 1500.0f, "Doctor fee getter works");
    
    // Conflict detection
    Appointment a1("A001", "P001", "D001", "10-10-2026", "09:00", "Pending");
    Appointment a2("A002", "P002", "D001", "10-10-2026", "09:00", "Pending");
    ASSERT(a1 == a2, "Appointment conflict detection (operator==)");
    
    Bill b("B001", "P001", "A001", 1500.0f, "Unpaid", "10-10-2026");
    ASSERT(b.getAmount() == 1500.0f, "Bill amount getter works");
    ASSERT(strCompare(b.getStatus(), "Unpaid") == 0, "Bill status getter works");
}

int main() {
    cout << "=======================================\n";
    cout << "Running MediCore Unit Tests...\n";
    cout << "=======================================\n";
    
    testUtils();
    testValidator();
    testStorage();
    testModels();
    
    cout << "\n=======================================\n";
    cout << "Tests Passed: " << testsPassed << " / " << testsRun << "\n";
    cout << "=======================================\n";
    
    return (testsPassed == testsRun) ? 0 : 1;
}
