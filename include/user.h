//
// Created by Stanislau Senkevich on 21.3.24.
//

#ifndef LAB_5_PROJECT_USER_H
#define LAB_5_PROJECT_USER_H

#include <string>
#include <iostream>
#include <utility>
#include <vector>
#include <sqlite3.h>
#include "exceptions.h"
#include "sql.h"

enum Role {
    GUEST = 0,
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

    User() : id(0), role(GUEST) {}

    User(int newId, std::string newLogin, std::string newPassHash, Role newRole)
            : id(newId), login(std::move(newLogin)), pass_hash(std::move(newPassHash)), role(newRole) {}

    [[nodiscard]] int getId() const {
        return id;
    }
    [[nodiscard]] std::string getLogin() const {
        return login;
    }
    [[nodiscard]] std::string getPassHash() const {
        return pass_hash;
    }
    [[nodiscard]] Role getRole() const {
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
    virtual int insertUserToDb(sqlite3 *db) {}
    virtual void getDataFromConsole();
};

#endif //LAB_5_PROJECT_USER_H
