//
// Created by Stanislau Senkevich on 21.3.24.
//

#ifndef LAB_5_PROJECT_DRIVER_H
#define LAB_5_PROJECT_DRIVER_H

#include "user.h"
#include <utility>
#include <vector>
#include <unordered_map>
#include <sstream>

class Driver : public User {

private:
    std::string name;
    std::string surname;
    std::vector<Category> category;
    int experience = 0;
    std::string address;
    std::string city;
    std::string birthday; // yyyy-mm-dd

public:

    Driver() = default;

    Driver(std::string newName, std::string newSurname,
           const std::vector<Category> &newCategories, int newExperience,
           std::string newAddress, std::string newCity,
           std::string newBirthday)
            : name(std::move(newName)), surname(std::move(newSurname)), category(newCategories),
              experience(newExperience), address(std::move(newAddress)), city(std::move(newCity)), birthday(std::move(newBirthday)) {}

    Driver(std::string newLogin, std::string newPassHash, std::string newName, std::string newSurname,
           const std::vector<Category> &newCategories, int newExperience,
           std::string newAddress, std::string newCity,
           std::string newBirthday)
            : name(std::move(newName)), surname(std::move(newSurname)), category(newCategories),
              experience(newExperience), address(std::move(newAddress)), city(std::move(newCity)), birthday(std::move(newBirthday)) {
                this->setLogin(newLogin);
                this->setPassHash(newPassHash);
              }

    [[nodiscard]] std::string getName() const {
        return name;
    }

    [[nodiscard]] std::string getSurname() const {
        return surname;
    }

    [[nodiscard]] std::vector<Category> getCategories() const {
        return category;
    }

    [[nodiscard]] std::string getCategoryString() const;

    [[nodiscard]] int getExperience() const {
        return experience;
    }

    [[nodiscard]] std::string getAddress() const {
        return address;
    }

    [[nodiscard]] std::string getCity() const {
        return city;
    }

    [[nodiscard]] std::string getBirthday() const {
        return birthday;
    }

    void setName(const std::string &newName) {
        name = newName;
    }

    void setSurname(const std::string &newSurname) {
        surname = newSurname;
    }

    void setCategory(const std::vector<Category> &newCategories) {
        category = newCategories;
    }

    void setCategoryFromStr(const std::string &str);

    void setExperience(int newExperience) {
        experience = newExperience;
    }

    void setAddress(const std::string &newAddress) {
        address = newAddress;
    }

    void setCity(const std::string &newCity) {
        city = newCity;
    }

    void setBirthday(const std::string &newBirthday) {
        birthday = newBirthday;
    }

    std::string print() const;

    void getDataFromDb(sqlite3 *db, int user_id) override;
    int insertUserToDb(sqlite3 *db) override;
    void getDataFromConsole() override;
};


#endif //LAB_5_PROJECT_DRIVER_H
