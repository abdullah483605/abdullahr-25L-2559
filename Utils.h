
// Utils.h - Manual string utility functions
// MediCore Hospital Management System
// All string operations implemented manually (no strcmp/strtok)

#ifndef UTILS_H
#define UTILS_H

#include <iostream>

// ---------------------------
// Manual string length
// ---------------------------
inline int strLen(const char* s) {
    if (!s) return 0;
    int len = 0;
    while (*(s + len) != '\0') {
        len++;
    }
    return len;
}

// ---------------------------
// Manual string copy
// ---------------------------
inline void strCopy(char* dest, const char* src) {
    if (!dest) return;
    if (!src) { *dest = '\0'; return; }
    int i = 0;
    while (*(src + i) != '\0') {
        *(dest + i) = *(src + i);
        i++;
    }
    *(dest + i) = '\0';
}

// ---------------------------
// Manual string compare (0 = equal)
// ---------------------------
inline int strCompare(const char* a, const char* b) {
    if (!a && !b) return 0;
    if (!a) return -1;
    if (!b) return 1;
    int i = 0;
    while (*(a + i) != '\0' && *(b + i) != '\0') {
        if (*(a + i) != *(b + i))
            return *(a + i) - *(b + i);
        i++;
    }
    return *(a + i) - *(b + i);
}

// ---------------------------
// Lowercase single character
// ---------------------------
inline char toLowerChar(char c) {
    if (c >= 'A' && c <= 'Z') return c + 32;
    return c;
}

// ---------------------------
// Case-insensitive string compare
// ---------------------------
inline int strCompareCI(const char* a, const char* b) {
    if (!a && !b) return 0;
    if (!a) return -1;
    if (!b) return 1;
    int i = 0;
    while (*(a + i) != '\0' && *(b + i) != '\0') {
        if (toLowerChar(*(a + i)) != toLowerChar(*(b + i)))
            return toLowerChar(*(a + i)) - toLowerChar(*(b + i));
        i++;
    }
    return toLowerChar(*(a + i)) - toLowerChar(*(b + i));
}

// ---------------------------
// Manual string concatenation
// ---------------------------
inline void strCat(char* dest, const char* src) {
    int dLen = strLen(dest);
    int i = 0;
    while (*(src + i) != '\0') {
        *(dest + dLen + i) = *(src + i);
        i++;
    }
    *(dest + dLen + i) = '\0';
}

// ---------------------------
// Manual char array to int
// ---------------------------
inline int charToInt(const char* s) {
    if (!s) return 0;
    int result = 0, i = 0, sign = 1;
    if (*s == '-') { sign = -1; i = 1; }
    while (*(s + i) >= '0' && *(s + i) <= '9') {
        result = result * 10 + (*(s + i) - '0');
        i++;
    }
    return result * sign;
}

// ---------------------------
// Manual int to char array
// ---------------------------
inline void intToChar(char* dest, int n) {
    if (n == 0) { *dest = '0'; *(dest + 1) = '\0'; return; }
    int idx = 0;
    if (n < 0) { *(dest + idx++) = '-'; n = -n; }
    int temp = n, digits = 0;
    while (temp > 0) { digits++; temp /= 10; }
    *(dest + idx + digits) = '\0';
    for (int i = digits - 1; i >= 0; i--) {
        *(dest + idx + i) = '0' + (n % 10);
        n /= 10;
    }
}

// ---------------------------
// Manual float to char (2 decimals)
// ---------------------------
inline void floatToChar(char* dest, float f) {
    if (f < 0) { *dest = '-'; f = -f; floatToChar(dest + 1, f); return; }
    int intPart = (int)f;
    int fracPart = (int)((f - intPart) * 100 + 0.5f);
    if (fracPart >= 100) { intPart++; fracPart -= 100; }
    intToChar(dest, intPart);
    int len = strLen(dest);
    *(dest + len) = '.';
    *(dest + len + 1) = '0' + (fracPart / 10);
    *(dest + len + 2) = '0' + (fracPart % 10);
    *(dest + len + 3) = '\0';
}

// ---------------------------
// Manual char array to float
// ---------------------------
inline float charToFloat(const char* s) {
    if (!s) return 0.0f;
    float result = 0.0f;
    int i = 0, sign = 1;
    if (*s == '-') { sign = -1; i = 1; }
    while (*(s + i) >= '0' && *(s + i) <= '9') {
        result = result * 10.0f + (*(s + i) - '0');
        i++;
    }
    if (*(s + i) == '.') {
        i++;
        float decimal = 0.1f;
        while (*(s + i) >= '0' && *(s + i) <= '9') {
            result += (*(s + i) - '0') * decimal;
            decimal *= 0.1f;
            i++;
        }
    }
    return result * sign;
}

// ---------------------------
// Case-insensitive substring search
// ---------------------------
inline bool strContainsCI(const char* haystack, const char* needle) {
    int hLen = strLen(haystack);
    int nLen = strLen(needle);
    if (nLen > hLen) return false;
    for (int i = 0; i <= hLen - nLen; i++) {
        bool found = true;
        for (int j = 0; j < nLen; j++) {
            if (toLowerChar(*(haystack + i + j)) != toLowerChar(*(needle + j))) {
                found = false;
                break;
            }
        }
        if (found) return true;
    }
    return false;
}

// ---------------------------
// Allocate and copy a char array
// ---------------------------
inline char* allocCopy(const char* src) {
    if (!src) {
        char* empty = new char[1];
        *empty = '\0';
        return empty;
    }
    int len = strLen(src);
    char* dest = new char[len + 1];
    strCopy(dest, src);
    return dest;
}

// ---------------------------
// Generate next ID (e.g., P001 -> P002)
// ---------------------------
inline void generateNextID(char* dest, const char* prefix, int num) {
    strCopy(dest, prefix);
    int pLen = strLen(prefix);
    if (num < 10) {
        *(dest + pLen) = '0';
        *(dest + pLen + 1) = '0';
        *(dest + pLen + 2) = '0' + num;
        *(dest + pLen + 3) = '\0';
    } else if (num < 100) {
        *(dest + pLen) = '0';
        *(dest + pLen + 1) = '0' + (num / 10);
        *(dest + pLen + 2) = '0' + (num % 10);
        *(dest + pLen + 3) = '\0';
    } else {
        *(dest + pLen) = '0' + (num / 100);
        *(dest + pLen + 1) = '0' + ((num / 10) % 10);
        *(dest + pLen + 2) = '0' + (num % 10);
        *(dest + pLen + 3) = '\0';
    }
}

// ---------------------------
// Parse a CSV field from a line
// Reads from line starting at pos, stops at delimiter or end
// Returns number of chars consumed
// ---------------------------
inline int parseField(const char* line, int pos, char* field, char delimiter = ',') {
    int i = 0;
    while (*(line + pos + i) != delimiter && *(line + pos + i) != '\0' && *(line + pos + i) != '\n' && *(line + pos + i) != '\r') {
        *(field + i) = *(line + pos + i);
        i++;
    }
    *(field + i) = '\0';
    if (*(line + pos + i) == delimiter) i++; // skip delimiter
    return i;
}

#endif
