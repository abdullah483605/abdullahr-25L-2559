// =============================================================
// gui_tests.cpp - Complete function test suite for MediCore
// Tests all backend functions WITHOUT needing SFML display
// =============================================================
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

int passed = 0, failed = 0, total = 0;

#define TEST(name, cond) do { \
    total++; \
    if (cond) { cout << "  [PASS] " << name << "\n"; passed++; } \
    else       { cout << "  [FAIL] " << name << "  <-- LINE " << __LINE__ << "\n"; failed++; } \
} while(0)

void printHeader(const char* section) {
    cout << "\n+--------------------------------------------------+\n";
    cout << "  " << section << "\n";
    cout << "+--------------------------------------------------+\n";
}

// ============================================================
void testUtils() {
    printHeader("1. Utils.h — Manual String Functions");
    TEST("strLen empty",          strLen("") == 0);
    TEST("strLen hello",          strLen("hello") == 5);
    TEST("strLen nullptr",        strLen(nullptr) == 0);

    char buf[50];
    strCopy(buf, "MediCore");
    TEST("strCopy normal",        strCompare(buf,"MediCore") == 0);
    strCopy(buf, nullptr);
    TEST("strCopy nullptr src",   buf[0] == '\0');

    TEST("strCompare equal",      strCompare("abc","abc") == 0);
    TEST("strCompare less",       strCompare("abc","abd") < 0);
    TEST("strCompare greater",    strCompare("abd","abc") > 0);
    TEST("strCompareCI upper",    strCompareCI("HELLO","hello") == 0);
    TEST("strContainsCI found",   strContainsCI("Cardiology","cardio") == true);
    TEST("strContainsCI notfound",strContainsCI("Cardiology","neuro") == false);

    char numBuf[20];
    intToChar(numBuf, 0);    TEST("intToChar zero",     strCompare(numBuf,"0")==0);
    intToChar(numBuf, 42);   TEST("intToChar positive", strCompare(numBuf,"42")==0);
    intToChar(numBuf, -7);   TEST("intToChar negative", strCompare(numBuf,"-7")==0);

    TEST("charToInt positive",    charToInt("123") == 123);
    TEST("charToInt negative",    charToInt("-55") == -55);
    TEST("charToInt zero",        charToInt("0") == 0);

    floatToChar(numBuf, 3.14f);
    TEST("floatToChar 3.14",      strLen(numBuf) > 3);
    TEST("charToFloat 3.14",      charToFloat("3.14") > 3.1f && charToFloat("3.14") < 3.2f);

    char idBuf[20];
    generateNextID(idBuf,"P",1);  TEST("generateNextID P001", strCompare(idBuf,"P001")==0);
    generateNextID(idBuf,"A",12); TEST("generateNextID A012", strCompare(idBuf,"A012")==0);
    generateNextID(idBuf,"B",100);TEST("generateNextID B100", strCompare(idBuf,"B100")==0);
}

// ============================================================
void testValidator() {
    printHeader("2. Validator — Input Validation");
    Validator v;

    TEST("validateID P001 valid",    v.validateID("P001","P") == true);
    TEST("validateID D001 valid",    v.validateID("D001","D") == true);
    TEST("validateID wrong prefix",  v.validateID("X001","P") == false);
    TEST("validateID no digits",     v.validateID("PABC","P") == false);
    TEST("validateID empty",         v.validateID("","P") == false);

    TEST("validateContact 11 digits",v.validateContact("03001234567") == true);
    TEST("validateContact too short",v.validateContact("0300123456") == false);
    TEST("validateContact too long", v.validateContact("030012345678") == false);

    TEST("validatePassword 6+chars", v.validatePassword("secret") == true);
    TEST("validatePassword 5 chars", v.validatePassword("pass1") == false);

    TEST("validateDate past",        v.validateDate("01-01-2020") == false);
    TEST("validateDate invalid fmt",  v.validateDate("2026/01/01") == false);
    TEST("validateDate invalid month",v.validateDate("01-13-2026") == false);

    TEST("validateTimeSlot 09:00",   v.validateTimeSlot("09:00") == true);
    TEST("validateTimeSlot 16:00",   v.validateTimeSlot("16:00") == true);
    TEST("validateTimeSlot invalid", v.validateTimeSlot("08:00") == false);
    TEST("validateTimeSlot bad fmt", v.validateTimeSlot("900") == false);

    TEST("validatePositiveFloat >0", v.validatePositiveFloat(100.0f) == true);
    TEST("validatePositiveFloat 0",  v.validatePositiveFloat(0.0f) == false);
    TEST("validatePositiveFloat neg",v.validatePositiveFloat(-1.0f) == false);

    TEST("validateNonEmpty filled",  v.validateNonEmpty("Hello") == true);
    TEST("validateNonEmpty empty",   v.validateNonEmpty("") == false);
}

// ============================================================
void testStorage() {
    printHeader("3. Storage<T> — Generic Template Container");
    Storage<Patient> store;

    TEST("Initial size=0",  store.size() == 0);
    TEST("findByID empty",  store.findByID("P001") == nullptr);

    Patient p1("P001","Alice",30,'F',"03001112222","pass12",5000.f);
    Patient p2("P002","Bob",  25,'M',"03003334444","pass12",3000.f);
    Patient p3("P003","Carol",22,'F',"03005556666","pass12",1000.f);

    TEST("add P001",        store.add(p1) == true);
    TEST("add P002",        store.add(p2) == true);
    TEST("add P003",        store.add(p3) == true);
    TEST("Size after 3",    store.size() == 3);

    Patient* fp = store.findByID("P002");
    TEST("findByID P002",   fp != nullptr);
    TEST("findByID name",   fp != nullptr && strCompare(fp->getName(),"Bob")==0);
    TEST("findByID missing",store.findByID("P999")==nullptr);

    TEST("at(0) id",        strCompare(store.at(0)->getID(),"P001")==0);
    TEST("at(2) id",        strCompare(store.at(2)->getID(),"P003")==0);
    TEST("at(-1) null",     store.at(-1)==nullptr);
    TEST("at(99) null",     store.at(99)==nullptr);

    store.removeByID("P002");
    TEST("Size after remove", store.size() == 2);
    TEST("P002 gone",         store.findByID("P002")==nullptr);
    TEST("P001 still exists", store.findByID("P001")!=nullptr);

    store.clear();
    TEST("clear() size=0",  store.size()==0);
}

// ============================================================
void testPatient() {
    printHeader("4. Patient — Operators, Login, CSV");

    Patient p("P001","Ahmed",28,'M',"03001234567","ahmed123",2000.f);
    TEST("getAge",          p.getAge() == 28);
    TEST("getGender",       p.getGender() == 'M');
    TEST("getBalance init", p.getBalance() == 2000.f);
    TEST("getRole",         strCompare(p.getRole(),"Patient")==0);

    p += 500.f;
    TEST("operator+= 500",  p.getBalance() == 2500.f);
    p -= 300.f;
    TEST("operator-= 300",  p.getBalance() == 2200.f);

    TEST("login valid",     p.login("P001","ahmed123") == true);
    TEST("login bad pass",  p.login("P001","wrong") == false);
    TEST("login bad id",    p.login("P002","ahmed123") == false);

    Patient p2("P001","X",20,'M',"03001234567","x",0);
    TEST("operator==",      (p == p2) == true); // same ID

    char csv[256]; p.toCSV(csv);
    TEST("toCSV has P001",  strContainsCI(csv,"P001")==true);
    TEST("toCSV has name",  strContainsCI(csv,"Ahmed")==true);
}

// ============================================================
void testDoctor() {
    printHeader("5. Doctor — Operators, Login, CSV");

    Doctor d("D001","Ayesha","Cardiology","03221234567","docpass",1500.f);
    TEST("getSpec",         strCompare(d.getSpecialization(),"Cardiology")==0);
    TEST("getFee",          d.getConsultationFee() == 1500.f);
    TEST("getRole",         strCompare(d.getRole(),"Doctor")==0);
    TEST("login valid",     d.login("D001","docpass") == true);
    TEST("login wrong",     d.login("D001","bad") == false);

    Doctor d2("D001","X","Y","Z","p",0);
    TEST("operator==",      (d == d2) == true);

    char csv[256]; d.toCSV(csv);
    TEST("toCSV has D001",  strContainsCI(csv,"D001")==true);
    TEST("toCSV has spec",  strContainsCI(csv,"Cardiology")==true);
}

// ============================================================
void testAdmin() {
    printHeader("6. Admin — Login, CSV");

    Admin a("ADM001","Admin","admin123");
    TEST("getRole",         strCompare(a.getRole(),"Admin")==0);
    TEST("login valid",     a.login("ADM001","admin123") == true);
    TEST("login wrong",     a.login("ADM001","bad") == false);

    char csv[256]; a.toCSV(csv);
    TEST("toCSV has id",    strContainsCI(csv,"ADM001")==true);
}

// ============================================================
void testAppointment() {
    printHeader("7. Appointment — Conflict Detection, CSV");

    Appointment a1("A001","P001","D001","10-10-2026","09:00","Pending");
    Appointment a2("A002","P002","D001","10-10-2026","09:00","Pending");
    Appointment a3("A003","P001","D001","10-10-2026","11:00","Pending");
    Appointment a4("A004","P001","D002","10-10-2026","09:00","Pending");
    Appointment a5("A005","P001","D001","10-10-2026","09:00","Cancelled");

    TEST("Conflict: same doc/date/slot",   (a1==a2)==true);
    TEST("No conflict: different slot",    (a1==a3)==false);
    TEST("No conflict: different doctor",  (a1==a4)==false);
    TEST("No conflict: one cancelled",     (a1==a5)==false);

    a1.setStatus("Completed");
    TEST("setStatus works",                strCompare(a1.getStatus(),"Completed")==0);

    char csv[256]; a1.toCSV(csv);
    TEST("toCSV has A001",  strContainsCI(csv,"A001")==true);
    TEST("toCSV has P001",  strContainsCI(csv,"P001")==true);
}

// ============================================================
void testBill() {
    printHeader("8. Bill — Amounts, Status, CSV");

    Bill b("B001","P001","A001",1500.f,"Unpaid","10-10-2026");
    TEST("getAmount",       b.getAmount() == 1500.f);
    TEST("getStatus",       strCompare(b.getStatus(),"Unpaid")==0);
    b.setStatus("Paid");
    TEST("setStatus Paid",  strCompare(b.getStatus(),"Paid")==0);

    char csv[256]; b.toCSV(csv);
    TEST("toCSV has B001",  strContainsCI(csv,"B001")==true);
}

// ============================================================
void testPrescription() {
    printHeader("9. Prescription — Fields, CSV");

    Prescription p("PR001","A001","P001","D001","10-10-2026","Paracetamol 500mg","Take after meals");
    TEST("getID",           strCompare(p.getID(),"PR001")==0);
    TEST("getMedicines",    strContainsCI(p.getMedicines(),"Paracetamol")==true);
    TEST("getPatientID",    strCompare(p.getPatientID(),"P001")==0);

    char csv[512]; p.toCSV(csv);
    TEST("toCSV has PR001", strContainsCI(csv,"PR001")==true);
    TEST("toCSV has meds",  strContainsCI(csv,"Paracetamol")==true);
}

// ============================================================
void testExceptions() {
    printHeader("10. Exception Hierarchy");

    bool caught1=false, caught2=false, caught3=false, caught4=false;
    try { throw InvalidInputException("bad input"); }
    catch(const HospitalException& e) { caught1 = strLen(e.what())>0; }

    try { throw InsufficientFundsException("no money"); }
    catch(const HospitalException& e) { caught2 = strLen(e.what())>0; }

    try { throw SlotUnavailableException("taken"); }
    catch(const HospitalException& e) { caught3 = strLen(e.what())>0; }

    try { throw FileNotFoundException("missing"); }
    catch(const HospitalException& e) { caught4 = strLen(e.what())>0; }

    TEST("InvalidInputException caught as HospitalException",      caught1);
    TEST("InsufficientFundsException caught as HospitalException", caught2);
    TEST("SlotUnavailableException caught as HospitalException",   caught3);
    TEST("FileNotFoundException caught as HospitalException",      caught4);
}

// ============================================================
void testFileHandler() {
    printHeader("11. FileHandler — Load, Append, Update");

    FileHandler fh("data/");

    Storage<Patient>     patients;
    Storage<Doctor>      doctors;
    Storage<Admin>       admins;
    Storage<Appointment> appts;
    Storage<Bill>        bills;
    Storage<Prescription> prescriptions;

    fh.loadPatients(&patients);
    fh.loadDoctors(&doctors);
    fh.loadAdmins(&admins);
    fh.loadAppointments(&appts);
    fh.loadBills(&bills);
    fh.loadPrescriptions(&prescriptions);

    TEST("Loaded >=3 patients",   patients.size() >= 3);
    TEST("Loaded >=3 doctors",    doctors.size() >= 3);
    TEST("Loaded >=1 admin",      admins.size() >= 1);
    TEST("Patient P001 exists",   patients.findByID("P001") != nullptr);
    TEST("Doctor D001 exists",    doctors.findByID("D001") != nullptr);
    TEST("Admin ADM001 exists",   admins.findByID("ADM001") != nullptr);

    // Verify fields parsed correctly
    Patient* p = patients.findByID("P001");
    TEST("P001 has name",         p != nullptr && strLen(p->getName()) > 0);
    TEST("P001 age > 0",          p != nullptr && p->getAge() > 0);
    TEST("P001 balance >= 0",     p != nullptr && p->getBalance() >= 0);

    Doctor* d = doctors.findByID("D001");
    TEST("D001 has spec",         d != nullptr && strLen(d->getSpecialization()) > 0);
    TEST("D001 fee > 0",          d != nullptr && d->getConsultationFee() > 0);
}

// ============================================================
int main() {
    cout << "\n";
    cout << "====================================================\n";
    cout << "   MediCore Hospital System — Full Test Suite\n";
    cout << "====================================================\n";

    testUtils();
    testValidator();
    testStorage();
    testPatient();
    testDoctor();
    testAdmin();
    testAppointment();
    testBill();
    testPrescription();
    testExceptions();
    testFileHandler();

    cout << "\n====================================================\n";
    cout << "  Results: " << passed << " PASSED, " << failed << " FAILED / " << total << " total\n";
    cout << "====================================================\n";

    return (failed == 0) ? 0 : 1;
}
