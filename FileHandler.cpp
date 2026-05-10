
// FileHandler.cpp - File I/O implementation (ONLY file handler)
// MediCore Hospital Management System

#include "../include/FileHandler.h"
#include "../include/Patient.h"
#include "../include/Doctor.h"
#include "../include/Admin.h"
#include "../include/Appointment.h"
#include "../include/Bill.h"
#include "../include/Prescription.h"
#include <fstream>
#include <ctime>

using namespace std;

// Default constructor - uses relative path
FileHandler::FileHandler() {
    strCopy(dataPath, "data/");
}

// Parameterized constructor
FileHandler::FileHandler(const char* path) {
    strCopy(dataPath, path);
}

FileHandler::~FileHandler() {}

// Build full file path
void FileHandler::buildPath(char* dest, const char* filename) const {
    strCopy(dest, dataPath);
    strCat(dest, filename);
}

const char* FileHandler::getDataPath() const { return dataPath; }


// Load patients from patients.txt
// Format: patient_id,name,age,gender,contact,password,balance

void FileHandler::loadPatients(Storage<Patient>* storage) {
    char filepath[256];
    buildPath(filepath, "patients.txt");
    ifstream file(filepath);
    if (!file.is_open()) return; // File may not exist yet

    char line[512];
    while (file.getline(line, 512)) {
        if (strLen(line) == 0) continue;

        char pid[30], nm[100], ageStr[10], gen[5], cont[20], pass[50], balStr[20];
        int pos = 0;
        pos += parseField(line, pos, pid);
        pos += parseField(line, pos, nm);
        pos += parseField(line, pos, ageStr);
        pos += parseField(line, pos, gen);
        pos += parseField(line, pos, cont);
        pos += parseField(line, pos, pass);
        pos += parseField(line, pos, balStr);

        Patient p(pid, nm, charToInt(ageStr), *gen, cont, pass, charToFloat(balStr));
        storage->add(p);
    }
    file.close();
}

// Load doctors from doctors.txt
// Format: doctor_id,name,specialization,contact,password,fee

void FileHandler::loadDoctors(Storage<Doctor>* storage) {
    char filepath[256];
    buildPath(filepath, "doctors.txt");
    ifstream file(filepath);
    if (!file.is_open()) return;

    char line[512];
    while (file.getline(line, 512)) {
        if (strLen(line) == 0) continue;

        char did[30], nm[100], spec[100], cont[20], pass[50], feeStr[20];
        int pos = 0;
        pos += parseField(line, pos, did);
        pos += parseField(line, pos, nm);
        pos += parseField(line, pos, spec);
        pos += parseField(line, pos, cont);
        pos += parseField(line, pos, pass);
        pos += parseField(line, pos, feeStr);

        Doctor d(did, nm, spec, cont, pass, charToFloat(feeStr));
        storage->add(d);
    }
    file.close();
}

// Load admins from admin.txt
// Format: admin_id,name,password

void FileHandler::loadAdmins(Storage<Admin>* storage) {
    char filepath[256];
    buildPath(filepath, "admin.txt");
    ifstream file(filepath);
    if (!file.is_open()) return;

    char line[512];
    while (file.getline(line, 512)) {
        if (strLen(line) == 0) continue;

        char aid[30], nm[100], pass[50];
        int pos = 0;
        pos += parseField(line, pos, aid);
        pos += parseField(line, pos, nm);
        pos += parseField(line, pos, pass);

        Admin a(aid, nm, pass);
        storage->add(a);
    }
    file.close();
}


// Load appointments
// Format: appointment_id,patient_id,doctor_id,date,time_slot,status

void FileHandler::loadAppointments(Storage<Appointment>* storage) {
    char filepath[256];
    buildPath(filepath, "appointments.txt");
    ifstream file(filepath);
    if (!file.is_open()) return;

    char line[512];
    while (file.getline(line, 512)) {
        if (strLen(line) == 0) continue;

        char aid[30], pid[30], did[30], dt[15], slot[10], stat[30];
        int pos = 0;
        pos += parseField(line, pos, aid);
        pos += parseField(line, pos, pid);
        pos += parseField(line, pos, did);
        pos += parseField(line, pos, dt);
        pos += parseField(line, pos, slot);
        pos += parseField(line, pos, stat);

        Appointment a(aid, pid, did, dt, slot, stat);
        storage->add(a);
    }
    file.close();
}


// Load bills
// Format: bill_id,patient_id,appointment_id,amount,status,date

void FileHandler::loadBills(Storage<Bill>* storage) {
    char filepath[256];
    buildPath(filepath, "bills.txt");
    ifstream file(filepath);
    if (!file.is_open()) return;

    char line[512];
    while (file.getline(line, 512)) {
        if (strLen(line) == 0) continue;

        char bid[30], pid[30], aid[30], amtStr[20], stat[30], dt[15];
        int pos = 0;
        pos += parseField(line, pos, bid);
        pos += parseField(line, pos, pid);
        pos += parseField(line, pos, aid);
        pos += parseField(line, pos, amtStr);
        pos += parseField(line, pos, stat);
        pos += parseField(line, pos, dt);

        Bill b(bid, pid, aid, charToFloat(amtStr), stat, dt);
        storage->add(b);
    }
    file.close();
}


// Load prescriptions
// Format: prescription_id,appointment_id,patient_id,doctor_id,date,medicines,notes

void FileHandler::loadPrescriptions(Storage<Prescription>* storage) {
    char filepath[256];
    buildPath(filepath, "prescriptions.txt");
    ifstream file(filepath);
    if (!file.is_open()) return;

    char line[1024];
    while (file.getline(line, 1024)) {
        if (strLen(line) == 0) continue;

        char prid[30], aid[30], pid[30], did[30], dt[15], meds[500], notes[300];
        int pos = 0;
        pos += parseField(line, pos, prid);
        pos += parseField(line, pos, aid);
        pos += parseField(line, pos, pid);
        pos += parseField(line, pos, did);
        pos += parseField(line, pos, dt);
        pos += parseField(line, pos, meds);
        pos += parseField(line, pos, notes);

        Prescription p(prid, aid, pid, did, dt, meds, notes);
        storage->add(p);
    }
    file.close();
}

// Append a single CSV record to file

void FileHandler::appendRecord(const char* filename, const char* csvLine) {
    char filepath[256];
    buildPath(filepath, filename);
    ofstream file(filepath, ios::app);
    if (!file.is_open()) {
        throw FileNotFoundException("Cannot open file for appending.");
    }
    file << csvLine << "\n";
    file.close();
}


// Update a record by ID (first field in CSV)

void FileHandler::updateRecordByID(const char* filename, const char* id, const char* newCSVLine) {
    char filepath[256];
    buildPath(filepath, filename);

    // Read all lines into a temp buffer
    ifstream inFile(filepath);
    if (!inFile.is_open()) {
        throw FileNotFoundException("Cannot open file for update.");
    }

    // We'll store lines dynamically
    char** lines = new char*[100];
    int lineCount = 0;
    char tempLine[1024];

    while (inFile.getline(tempLine, 1024) && lineCount < 100) {
        if (strLen(tempLine) == 0) continue;
        // Extract first field (ID)
        char firstField[30];
        parseField(tempLine, 0, firstField);

        if (strCompare(firstField, id) == 0) {
            // Replace this line
            *(lines + lineCount) = allocCopy(newCSVLine);
        } else {
            *(lines + lineCount) = allocCopy(tempLine);
        }
        lineCount++;
    }
    inFile.close();

    // Rewrite file
    ofstream outFile(filepath, ios::trunc);
    if (!outFile.is_open()) {
        // Clean up
        for (int i = 0; i < lineCount; i++) delete[] *(lines + i);
        delete[] lines;
        throw FileNotFoundException("Cannot open file for writing.");
    }
    for (int i = 0; i < lineCount; i++) {
        outFile << *(lines + i) << "\n";
        delete[] *(lines + i);
    }
    outFile.close();
    delete[] lines;
}


// Delete a record by ID

void FileHandler::deleteRecordByID(const char* filename, const char* id) {
    char filepath[256];
    buildPath(filepath, filename);

    ifstream inFile(filepath);
    if (!inFile.is_open()) return;

    char** lines = new char*[100];
    int lineCount = 0;
    char tempLine[1024];

    while (inFile.getline(tempLine, 1024) && lineCount < 100) {
        if (strLen(tempLine) == 0) continue;
        char firstField[30];
        parseField(tempLine, 0, firstField);
        if (strCompare(firstField, id) != 0) {
            *(lines + lineCount) = allocCopy(tempLine);
            lineCount++;
        }
    }
    inFile.close();

    ofstream outFile(filepath, ios::trunc);
    for (int i = 0; i < lineCount; i++) {
        outFile << *(lines + i) << "\n";
        delete[] *(lines + i);
    }
    outFile.close();
    delete[] lines;
}


// Rewrite entire patients file

void FileHandler::rewritePatients(Storage<Patient>* storage) {
    char filepath[256];
    buildPath(filepath, "patients.txt");
    ofstream file(filepath, ios::trunc);
    if (!file.is_open()) throw FileNotFoundException("Cannot open patients.txt");

    char buffer[512];
    for (int i = 0; i < storage->size(); i++) {
        storage->at(i)->toCSV(buffer);
        file << buffer << "\n";
    }
    file.close();
}


// Rewrite entire doctors file

void FileHandler::rewriteDoctors(Storage<Doctor>* storage) {
    char filepath[256];
    buildPath(filepath, "doctors.txt");
    ofstream file(filepath, ios::trunc);
    if (!file.is_open()) throw FileNotFoundException("Cannot open doctors.txt");

    char buffer[512];
    for (int i = 0; i < storage->size(); i++) {
        storage->at(i)->toCSV(buffer);
        file << buffer << "\n";
    }
    file.close();
}

// Rewrite entire appointments file

void FileHandler::rewriteAppointments(Storage<Appointment>* storage) {
    char filepath[256];
    buildPath(filepath, "appointments.txt");
    ofstream file(filepath, ios::trunc);
    if (!file.is_open()) throw FileNotFoundException("Cannot open appointments.txt");

    char buffer[512];
    for (int i = 0; i < storage->size(); i++) {
        storage->at(i)->toCSV(buffer);
        file << buffer << "\n";
    }
    file.close();
}

// Rewrite entire bills file

void FileHandler::rewriteBills(Storage<Bill>* storage) {
    char filepath[256];
    buildPath(filepath, "bills.txt");
    ofstream file(filepath, ios::trunc);
    if (!file.is_open()) throw FileNotFoundException("Cannot open bills.txt");

    char buffer[512];
    for (int i = 0; i < storage->size(); i++) {
        storage->at(i)->toCSV(buffer);
        file << buffer << "\n";
    }
    file.close();
}


// Rewrite entire prescriptions file

void FileHandler::rewritePrescriptions(Storage<Prescription>* storage) {
    char filepath[256];
    buildPath(filepath, "prescriptions.txt");
    ofstream file(filepath, ios::trunc);
    if (!file.is_open()) throw FileNotFoundException("Cannot open prescriptions.txt");

    char buffer[1024];
    for (int i = 0; i < storage->size(); i++) {
        storage->at(i)->toCSV(buffer);
        file << buffer << "\n";
    }
    file.close();
}


// Log security event

void FileHandler::logSecurity(const char* role, const char* enteredID, const char* result) {
    char filepath[256];
    buildPath(filepath, "security_log.txt");

    // Get current timestamp
    time_t now = time(nullptr);
    struct tm* t = localtime(&now);
    char timestamp[30];
    // Format: DD-MM-YYYY HH:MM:SS
    int day = t->tm_mday, month = t->tm_mon + 1, year = t->tm_year + 1900;
    int hour = t->tm_hour, minute = t->tm_min, sec = t->tm_sec;
    *(timestamp + 0) = '0' + (day / 10); *(timestamp + 1) = '0' + (day % 10);
    *(timestamp + 2) = '-';
    *(timestamp + 3) = '0' + (month / 10); *(timestamp + 4) = '0' + (month % 10);
    *(timestamp + 5) = '-';
    intToChar(timestamp + 6, year);
    *(timestamp + 10) = ' ';
    *(timestamp + 11) = '0' + (hour / 10); *(timestamp + 12) = '0' + (hour % 10);
    *(timestamp + 13) = ':';
    *(timestamp + 14) = '0' + (minute / 10); *(timestamp + 15) = '0' + (minute % 10);
    *(timestamp + 16) = ':';
    *(timestamp + 17) = '0' + (sec / 10); *(timestamp + 18) = '0' + (sec % 10);
    *(timestamp + 19) = '\0';

    char logLine[300];
    strCopy(logLine, timestamp);
    strCat(logLine, ",");
    strCat(logLine, role);
    strCat(logLine, ",");
    strCat(logLine, enteredID);
    strCat(logLine, ",");
    strCat(logLine, result);

    ofstream file(filepath, ios::app);
    if (file.is_open()) {
        file << logLine << "\n";
        file.close();
    }
}


// Append to discharged file
void FileHandler::appendDischarged(const char* record) {
    char filepath[256];
    buildPath(filepath, "discharged.txt");
    ofstream file(filepath, ios::app);
    if (file.is_open()) {
        file << record << "\n";
        file.close();
    }
}

// Read and display security log
void FileHandler::readSecurityLog() const {
    char filepath[256];
    buildPath(filepath, "security_log.txt");
    ifstream file(filepath);
    if (!file.is_open()) {
        cout << "  No security log found.\n";
        return;
    }

    cout << "\n+==============================================================+\n";
    cout << "|                    SECURITY LOG                              |\n";
    cout << "+==============================================================+\n";
    cout << "| Timestamp            | Role     | ID         | Result        |\n";
    cout << "+----------------------+----------+------------+---------------+\n";

    char line[512];
    while (file.getline(line, 512)) {
        if (strLen(line) == 0) continue;
        char ts[30], role[20], eid[30], res[30];
        int pos = 0;
        pos += parseField(line, pos, ts);
        pos += parseField(line, pos, role);
        pos += parseField(line, pos, eid);
        pos += parseField(line, pos, res);
        cout << "| " << ts << " | " << role << " | " << eid << " | " << res << "\n";
    }
    cout << "+==============================================================+\n";
    file.close();
}
