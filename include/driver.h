//
// Created by hakeyn on 21.3.24.
//

#ifndef LAB_5_PROJECT_DRIVER_H
#define LAB_5_PROJECT_DRIVER_H

#include "user.h"
#include <vector>

class Driver : public User {

private:
    std::string name;
    std::string surname;
    std::vector<Category> category;
    int experience;
    std::string address;
    std::string city;
    std::string birthday; // yyyy-mm-dd

public:

    Driver() {};

    std::string getName() const {
        return name;
    }
    std::string getSurname() const {
        return surname;
    }
    std::vector<Category> getCategories() const {
        return category;
    }
    int getExperience() const {
        return experience;
    }
    std::string getAddress() const {
        return address;
    }
    std::string getCity() const {
        return city;
    }
    std::string getBirthday() const {
        return birthday;
    }

    void setName(const std::string& newName) {
        name = newName;
    }
    void setSurname(const std::string& newSurname) {
        surname = newSurname;
    }
    void setCategories(const std::vector<Category>& newCategories) {
        category = newCategories;
    }
    void setExperience(int newExperience) {
        experience = newExperience;
    }
    void setAddress(const std::string& newAddress) {
        address = newAddress;
    }
    void setCity(const std::string& newCity) {
        city = newCity;
    }
    void setBirthday(const std::string& newBirthday) {
        birthday = newBirthday;
    }

    void getDataFromSQL(sqlite3* db, int user_id);
};


#endif //LAB_5_PROJECT_DRIVER_H
