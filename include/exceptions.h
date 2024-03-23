//
// Created by hakeyn on 21.3.24.
//

#ifndef LAB_5_PROJECT_EXCEPTIONS_H
#define LAB_5_PROJECT_EXCEPTIONS_H

#include <exception>
#include <utility>

class PermissionDeniedException : public std::exception {
public:
    char* what() {
        return "Permission denied\n";
    }
};

class InternalErrorException : public std::exception {
private:
    std::string msg;
public:
    explicit InternalErrorException(std::string msg) {this->msg = std::move(msg);};
    char* what() {
        return const_cast<char *>(msg.c_str());
    }

};

#endif //LAB_5_PROJECT_EXCEPTIONS_H
