//
// Created by hakeyn on 21.3.24.
//

#ifndef LAB_5_PROJECT_USER_H
#define LAB_5_PROJECT_USER_H

#include <string>
#include <vector>
#include <sqlite3.h>
#include "exceptions.h"

enum Role {
    DRIVER = 1,
    DISPATCHER = 2,
    ADMIN = 3,
};

enum Category {
    A = 0,
    B = 1,
    C = 2,
    D = 3,
    BE = 4,
    CE = 5,
};

class User {
private:
    int id;
    std::string login;
    std::string pass_hash;
    Role role;

public:

    User() : id(0), login(""), pass_hash(""), role(DRIVER) {}

    User(int newId, const std::string &newLogin, const std::string &newPassHash, Role newRole)
            : id(newId), login(newLogin), pass_hash(newPassHash), role(newRole) {}

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

    void setLogin(const std::string &newLogin) {
        login = newLogin;
    }

    void setPassHash(const std::string &newPassHash) {
        pass_hash = newPassHash;
    }

    void setRole(Role newRole) {
        role = newRole;
    }

    static std::string toLower(std::string str);

    virtual void getDataFromDb(sqlite3 *db, int user_id);

    virtual void insertUserToDb(sqlite3 *db) {}
};

#endif //LAB_5_PROJECT_USER_H
