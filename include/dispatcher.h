//
// Created by Stanislau Senkevich on 21.3.24.
//

#ifndef LAB_5_PROJECT_DISPATCHER_H
#define LAB_5_PROJECT_DISPATCHER_H

#include <utility>

#include "user.h"

class Dispatcher : public User {
private:
    std::string name;
    std::string surname;
    std::string address;
    std::string city;

public:
    Dispatcher() = default;

    Dispatcher(std::string newName, std::string newSurname,
               std::string newAddress, std::string newCity)
            : name(std::move(newName)), surname(std::move(newSurname)), address(std::move(newAddress)), city(std::move(newCity)) {}

    [[nodiscard]] std::string getName() const {
        return name;
    }
    [[nodiscard]] std::string getSurname() const {
        return surname;
    }
    [[nodiscard]] std::string getAddress() const {
        return address;
    }
    [[nodiscard]] std::string getCity() const {
        return city;
    }
    void setName(const std::string &newName) {
        name = newName;
    }
    void setSurname(const std::string &newSurname) {
        surname = newSurname;
    }
    void setAddress(const std::string &newAddress) {
        address = newAddress;
    }
    void setCity(const std::string &newCity) {
        city = newCity;
    }
    void getDataFromDb(sqlite3 *db, int user_id) override;

    int insertUserToDb(sqlite3 *db) override;
    void getDataFromConsole() override;
};

#endif //LAB_5_PROJECT_DISPATCHER_H
