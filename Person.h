
// Person.h - Abstract base class for all users
// MediCore Hospital Management System

#ifndef PERSON_H
#define PERSON_H

#include <iostream>
#include "Utils.h"

class Person {
protected:
    char* id;
    char* name;
    char* contact;
    char* password;

public:
    // Constructors and destructor
    Person();
    Person(const char* id, const char* name, const char* contact, const char* password);
    Person(const Person& other);
    Person& operator=(const Person& other);
    virtual ~Person();

    // Getters
    const char* getID() const;
    const char* getName() const;
    const char* getContact() const;
    const char* getPassword() const;

    // Setters
    void setID(const char* newID);
    void setName(const char* newName);
    void setContact(const char* newContact);
    void setPassword(const char* newPass);

    // Pure virtual functions (polymorphism + abstraction)
    virtual void display() const = 0;
    virtual const char* getRole() const = 0;
};

#endif
