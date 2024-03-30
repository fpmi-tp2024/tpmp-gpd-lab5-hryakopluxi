//
// Created by Stanislau Senkevich on 21.3.24.
//

#ifndef LAB_5_PROJECT_VALIDATOR_H
#define LAB_5_PROJECT_VALIDATOR_H

#include "config.h"
#include "order.h"
#include "driver.h"
#include "dispatcher.h"
#include "exceptions.h"
#include "car.h"
#include <regex>
#include <ctime>
#include <chrono>
#include <sstream>
#include <openssl/evp.h>
#include <iomanip>

class Validator {
private:
    static const std::regex NAME_PATTERN;
    static const std::regex ADDRESS_PATTERN;
    static const std::regex CITY_PATTERN;
    static const std::regex DATE_PATTERN;
    static const std::regex LICENSE_PATTERN;
    static const std::regex LOGIN_PATTERN;
    static const std::regex PASSWORD_PATTERN;

    static const std::string INVALID_LOGIN_MSG;
    static const std::string INVALID_PASSWORD_MSG;
    static const std::string INVALID_DATE_MSG;
    static const std::string INVALID_AGE_MSG;
    static const std::string INVALID_LICENSE_MSG;
    static const std::string INVALID_ADDRESS_MSG;
    static const std::string INVALID_CITY_MSG;
    static const std::string INVALID_NAME_MSG;
    static const std::string INVALID_SURNAME_MSG;
    static const std::string INVALID_EXPERIENCE_MSG;
    static const std::string INVALID_LOAD_MSG;
    static const std::string INVALID_LOAD_CAPACITY_MSG;
    static const std::string INVALID_COST_MSG;
    static const std::string INVALID_MILEAGE_MSG;
    static const std::string DRIVER_NOT_FOUND_MSG;
    static const std::string DISPATCHER_NOT_FOUND_MSG;
    static const std::string CAR_NOT_FOUND_MSG;
    static const std::string ORDER_NOT_FOUND_MSG;
    static const std::string USER_NOT_FOUND_MSG;
    static const std::string DRIVER_NOT_OBTAIN_CAR_MSG;
    static const std::string LOAD_TOO_BIG_MSG;
public:
    static bool validName(const std::string& name);
    static bool validSurname(const std::string& surname);
    static bool validAddress(const std::string& address);
    static bool validCity(const std::string& city);
    static bool validDate(const std::string& date);
    static bool validLicense(const std::string& license);
    static bool validLogin(const std::string& name);
    static bool validPassword(const std::string& name);

    static bool validPeriod(const std::string& date_start, const std::string& date_end);
    static bool validAge(const std::string& date);

    static bool validDriver(const Driver& driver);
    static bool validDispatcher(const Dispatcher& dispatcher);
    static bool validOrder(const Order& order, sqlite3* db);
    static bool validCar(const Car& car, sqlite3* db);

    static void validateUpdateCar(Car& update, int car_id, sqlite3* db);
    static void validateUpdateDriver(Driver& update, int driver_id, sqlite3* db);
    static void validateUpdateDispatcher(Dispatcher& update, int dispatcher_id, sqlite3* db);
    static void validateUpdateOrder(Order& update, int order_id, sqlite3* db);
    static void validateUpdateUser(User& update, int user_id, sqlite3* db);

    [[nodiscard]] static std::string hashPassword(const std::string& password) ;
    static bool validatePassword(const std::string& hash_pass, const std::string& provided_pass);
};

#endif //LAB_5_PROJECT_VALIDATOR_H
