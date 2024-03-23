//
// Created by hakeyn on 21.3.24.
//

#ifndef LAB_5_PROJECT_USER_H
#define LAB_5_PROJECT_USER_H

#include <string>
#include <vector>
#include <sqlite3.h>

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

    virtual std::string getName() const { return ""; }

    virtual std::string getSurname() const { return ""; }

    virtual std::vector<Category> getCategories() const { return std::vector<Category>(0); }

    virtual std::string getCategoryString() const { return ""; }

    virtual int getExperience() const { return -1; }

    virtual std::string getAddress() const { return ""; }

    virtual std::string getCity() const {return "";};

    virtual std::string getBirthday() const { return ""; }

    virtual void setName(const std::string &newName) {};

    virtual void setSurname(const std::string &newSurname) {};

    virtual void setCategory(const std::vector<Category> &newCategories) {};

    virtual void setExperience(int newExperience) {};

    virtual void setAddress(const std::string &newAddress) {};

    virtual void setCity(const std::string &newCity) {};

    virtual void setBirthday(const std::string &newBirthday) {};

    static std::string toLower(std::string str);

    virtual void getDataFromDb(sqlite3 *db, int user_id) {}

    virtual void insertUserToDb(sqlite3 *db) {}
};

#endif //LAB_5_PROJECT_USER_H
