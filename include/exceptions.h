//
// Created by hakeyn on 21.3.24.
//

#ifndef LAB_5_PROJECT_EXCEPTIONS_H
#define LAB_5_PROJECT_EXCEPTIONS_H

#include <exception>
#include <utility>

class PermissionDeniedException : public std::exception {
private:
    std::string msg = "Permission denied\n";
public:
    PermissionDeniedException() = default;
    explicit PermissionDeniedException(std::string msg) : msg(std::move(msg)) {}
    PermissionDeniedException(const PermissionDeniedException& e) {}
    [[nodiscard]] const char* what() const noexcept override {
        return msg.c_str();
    }
};

class InternalErrorException : public std::exception {
private:
    std::string msg;
public:
    explicit InternalErrorException(std::string msg) : msg(std::move(msg)) {}
    InternalErrorException(const InternalErrorException& e) {
        msg = e.msg;
    }

    [[nodiscard]] const char* what() const noexcept override {
        return msg.c_str();
    }
};

#endif //LAB_5_PROJECT_EXCEPTIONS_H
