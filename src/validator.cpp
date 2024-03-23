//
// Created by hakeyn on 21.3.24.
//

#include "../include/validator.h"
#include <iostream>

const std::regex Validator::name_pattern(R"(^[A-Za-zА-Яа-я]+$)");
const std::regex Validator::address_pattern(R"(^[a-zA-Zа-яА-Я0-9\s.,/]+$)");
const std::regex Validator::city_pattern(R"(^[A-Za-z]+$)");
const std::regex Validator::date_pattern(R"(^\d{4}-\d{2}-\d{2}$)");

bool Validator::validLicense(std::string license) {
    std::regex license_pattern("^[0-9]{4}[ABEIKMHOPCTX]{2}-[1-7]$");
    return std::regex_match(license, license_pattern);
}

bool Validator::validAge(std::string date_str) {
    std::tm date = {};
    std::istringstream ss(date_str);
    ss >> date.tm_year;  // Year
    ss.ignore();  // Ignore the dash '-'
    ss >> date.tm_mon;   // Month
    ss.ignore();  // Ignore the dash '-'
    ss >> date.tm_mday; // Day

    auto now = std::chrono::system_clock::now();
    auto currentTime = std::chrono::system_clock::to_time_t(now);
    std::tm current = *std::localtime(&currentTime);

    current.tm_year -= 18 - 1900;
    return std::mktime(&date) <= std::mktime(&current);
}

bool Validator::validDriver(const Driver &driver) {
    if (driver.getLogin().empty()) {
        throw std::invalid_argument("Login must not be empty\n");
    }
    if (driver.getPassHash().empty()) {
        throw std::invalid_argument("Password must not be empty\n");
    }
    if (!std::regex_match(driver.getName(), name_pattern)) {
        throw std::invalid_argument("Name must consist only of russian or latin letters\n");
    }
    if (!std::regex_match(driver.getName(), name_pattern)) {
        throw std::invalid_argument("Surname must consist only of russian or latin letters\n");
    }
    if (driver.getExperience() < 0) {
        throw std::invalid_argument("Experience cannot be negative\n");
    }

    if (!std::regex_match(driver.getAddress(), address_pattern)) {
        throw std::invalid_argument("Address must consist only of russian or latin letters\n");
    }

    if (!std::regex_match(driver.getCity(), city_pattern)) {
        throw std::invalid_argument("City must consist only of latin letters\n");
    }

    if (!std::regex_match(driver.getBirthday(), date_pattern)) {
        throw std::invalid_argument("Birthday must satisfy format YYYY-MM-DD\n");
    }

    if (!validAge(driver.getBirthday())) {
        throw std::invalid_argument("Age must be >= 18 years\n");
    }

    return true;
}