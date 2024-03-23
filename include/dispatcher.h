//
// Created by hakeyn on 21.3.24.
//

#ifndef LAB_5_PROJECT_DISPATCHER_H
#define LAB_5_PROJECT_DISPATCHER_H

#include "user.h"

class Dispatcher : public User {
private:
    std::string name;
    std::string surname;
    std::string address;
    std::string city;

public:
    std::string getName() const {
        return name;
    }
    std::string getSurname() const {
        return surname;
    }
    std::string getAddress() const {
        return address;
    }
    std::string getCity() const {
        return city;
    }

    void setName(const std::string& newName) {
        name = newName;
    }
    void setSurname(const std::string& newSurname) {
        surname = newSurname;
    }
    void setAddress(const std::string& newAddress) {
        address = newAddress;
    }
    void setCity(const std::string& newCity) {
        city = newCity;
    }

    void getDataFromDb(sqlite3* db, int user_id);
};

#endif //LAB_5_PROJECT_DISPATCHER_H
