//
// Created by hakeyn on 21.3.24.
//

#ifndef LAB_5_PROJECT_VALIDATOR_H
#define LAB_5_PROJECT_VALIDATOR_H

#include "config.h"
#include "order.h"
#include "driver.h"
#include "exceptions.h"
#include "car.h"
#include <regex>
#include <ctime>
#include <chrono>
#include <sstream>

class Validator {
private:;
    static const std::regex name_pattern;
    static const std::regex address_pattern;
    static const std::regex city_pattern;
    static const std::regex date_pattern;
public:
    static bool validOrder(const Order& order, sqlite3* db);
    static bool validDriver(const Driver& driver);
    static bool validLicense(std::string license);
    static bool validDate(std::string date);
    static bool validAge(std::string date);
};

#endif //LAB_5_PROJECT_VALIDATOR_H
