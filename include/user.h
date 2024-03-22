//
// Created by hakeyn on 21.3.24.
//

#ifndef LAB_5_PROJECT_USER_H
#define LAB_5_PROJECT_USER_H

#include <string>
#include <vector>

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

    virtual std::string getName() const {};

    std::string getSurname() const{};
    virtual std::vector<Category> getCategories() const{};
    virtual int getExperience() const{};
    virtual std::string getAddress() const{};
    std::string getCity() const{};
    virtual std::string getBirthday() const{};

    virtual void setName(const std::string& newName){};
    virtual void setSurname(const std::string& newSurname){};
    virtual void setCategories(const std::vector<Category>& newCategories){};
    virtual void setExperience(int newExperience){};
    virtual void setAddress(const std::string& newAddress){};
    virtual void setCity(const std::string& newCity){};
    virtual void setBirthday(const std::string& newBirthday){};

    virtual void getDataFromSQL(sqlite3* db, int user_id){};
};

#endif //LAB_5_PROJECT_USER_H
