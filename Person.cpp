
// Person.cpp - Abstract base class implementation
// MediCore Hospital Management System

#include "../include/Person.h"

// Default constructor
Person::Person() {
    id = allocCopy("");
    name = allocCopy("");
    contact = allocCopy("");
    password = allocCopy("");
}

// Parameterized constructor
Person::Person(const char* id, const char* name, const char* contact, const char* password) {
    this->id = allocCopy(id);
    this->name = allocCopy(name);
    this->contact = allocCopy(contact);
    this->password = allocCopy(password);
}

// Copy constructor
Person::Person(const Person& other) {
    id = allocCopy(other.id);
    name = allocCopy(other.name);
    contact = allocCopy(other.contact);
    password = allocCopy(other.password);
}

// Assignment operator
Person& Person::operator=(const Person& other) {
    if (this != &other) {
        delete[] id;
        delete[] name;
        delete[] contact;
        delete[] password;
        id = allocCopy(other.id);
        name = allocCopy(other.name);
        contact = allocCopy(other.contact);
        password = allocCopy(other.password);
    }
    return *this;
}

// Virtual destructor
Person::~Person() {
    delete[] id;
    delete[] name;
    delete[] contact;
    delete[] password;
}

// Getters
const char* Person::getID() const { return id; }
const char* Person::getName() const { return name; }
const char* Person::getContact() const { return contact; }
const char* Person::getPassword() const { return password; }

// Setters
void Person::setID(const char* newID) {
    delete[] id;
    id = allocCopy(newID);
}
void Person::setName(const char* newName) {
    delete[] name;
    name = allocCopy(newName);
}
void Person::setContact(const char* newContact) {
    delete[] contact;
    contact = allocCopy(newContact);
}
void Person::setPassword(const char* newPass) {
    delete[] password;
    password = allocCopy(newPass);
}
