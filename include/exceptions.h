//
// Created by hakeyn on 21.3.24.
//

#ifndef LAB_5_PROJECT_EXCEPTIONS_H
#define LAB_5_PROJECT_EXCEPTIONS_H

#include <exception>

class PermissionDeniedException : public std::exception {
public:
    char* what() {
        return "Permission denied\n";
    }
};

class InvalidCarLicenseException : public std::exception {
public:
    char* what() {
        return "Invalid car license\n";
    }
};

class InternalErrorException : public std::exception {
public:
    char* what() {
        return "Something went wrong\n";
    }

};

#endif //LAB_5_PROJECT_EXCEPTIONS_H
