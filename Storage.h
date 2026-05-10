
// Storage.h - Generic template storage class
// MediCore Hospital Management System
// Static array allowed ONLY inside this class (per requirements)

#ifndef STORAGE_H
#define STORAGE_H

#include "Utils.h"

template <typename T>
class Storage {
private:
    T data[100];   // Static array (only allowed here)
    int count;

public:
    // Constructor
    Storage() : count(0) {}

    // Add a record to storage
    bool add(const T& item) {
        if (count >= 100) return false;
        *(data + count) = item;
        count++;
        return true;
    }

    // Remove a record by ID
    bool removeByID(const char* id) {
        for (int i = 0; i < count; i++) {
            if (strCompare((data + i)->getID(), id) == 0) {
                // Shift elements left to fill gap
                for (int j = i; j < count - 1; j++) {
                    *(data + j) = *(data + j + 1);
                }
                count--;
                return true;
            }
        }
        return false;
    }

    // Find a record by ID (returns pointer or nullptr)
    T* findByID(const char* id) {
        for (int i = 0; i < count; i++) {
            if (strCompare((data + i)->getID(), id) == 0) {
                return (data + i);
            }
        }
        return nullptr;
    }

    // Get pointer to internal array
    T* getAll() {
        return data;
    }

    // Get current count
    int size() const {
        return count;
    }

    // Clear all records
    void clear() {
        count = 0;
    }

    // Get element at index
    T* at(int index) {
        if (index >= 0 && index < count) {
            return (data + index);
        }
        return nullptr;
    }
};

#endif
