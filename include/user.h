//
// Created by hakeyn on 21.3.24.
//

#ifndef LAB_5_PROJECT_USER_H
#define LAB_5_PROJECT_USER_H

#include <string>

enum Role {
    GUEST = 0,
    DRIVER = 1,
    DISPATCHER = 2,
    ADMIN = 3,
};

class User {
private:
    int id;
    std::string login;
    std::string pass_hash;
    Role role;

public:
    int getId() const {
        return id;
    }

    std::string getLogin() const {
        return login;
    }

    std::string getPassHash() const {
        return pass_hash;
    }

    Role getRole() const {
        return role;
    }

    void setId(int newId) {
        id = newId;
    }

    void setLogin(const std::string& newLogin) {
        login = newLogin;
    }

    void setPassHash(const std::string& newPassHash) {
        pass_hash = newPassHash;
    }

    void setRole(Role newRole) {
        role = newRole;
    }

    void clear() {
        id = 0;
        login = "";
        pass_hash = "";
        role = GUEST;
    }
};

#endif //LAB_5_PROJECT_USER_H
