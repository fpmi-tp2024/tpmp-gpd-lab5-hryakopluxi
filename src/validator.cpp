//
// Created by hakeyn on 21.3.24.
//

#include "../include/validator.h"
#include <iostream>

const std::regex Validator::name_pattern(R"(^[A-Za-z]+$)");
const std::regex Validator::address_pattern(R"(^[a-zA-Z0-9\s.,/]+$)");
const std::regex Validator::city_pattern(R"(^[A-Za-z]+$)");
const std::regex Validator::date_pattern(R"(^\d{4}-\d{2}-\d{2}$)");
const std::regex Validator::license_pattern("^[0-9]{4}[ABEIKMHOPCTX]{2}-[1-7]$");

bool Validator::validLicense(const std::string &license) {
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

bool Validator::validDate(std::string date_str) {
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

    current.tm_year += 1902;
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
        throw std::invalid_argument("Name must consist only of latin characters\n");
    }
    if (!std::regex_match(driver.getSurname(), name_pattern)) {
        throw std::invalid_argument("Surname must consist only of latin characters\n");
    }
    if (driver.getExperience() < 0) {
        throw std::invalid_argument("Experience cannot be negative\n");
    }

    if (!std::regex_match(driver.getAddress(), address_pattern)) {
        throw std::invalid_argument("Address must consist only of latin characters\n");
    }

    if (!std::regex_match(driver.getCity(), city_pattern)) {
        throw std::invalid_argument("City must consist only of latin characters\n");
    }

    if (!std::regex_match(driver.getBirthday(), date_pattern)) {
        throw std::invalid_argument("Birthday must satisfy format YYYY-MM-DD\n");
    }

    if (!validAge(driver.getBirthday())) {
        throw std::invalid_argument("Age must be >= 18 years\n");
    }

    return true;
}

bool Validator::validOrder(const Order &order, sqlite3 *db) {
    Driver d;
    Car c;
    try {
        d.getDataFromDb(db, order.getDriverId());
    } catch (const std::exception &e) {
        throw std::invalid_argument("Driver doesn't exists");
    }

    try {
        c.getDataFromDb(db, order.getCarId());
    } catch (const std::exception &e) {
        throw std::invalid_argument("Car doesn't exists");
    }

    if (c.getDriverId() != d.getId()) {
        throw std::invalid_argument("Driver doesn't obtain the car\n");
    }

    if (c.getLoadCapacity() < order.getLoad()) {
        throw std::invalid_argument("Load of cargo > load capacity of the car\n");
    }

    if (!std::regex_match(order.getDate(), date_pattern)) {
        throw std::invalid_argument("Date does not satisfy pattern YYYY-MM-DD\n");
    }

    if (!validDate(order.getDate())) {
        throw std::invalid_argument("Invalid date was provided\n");
    }

    if (order.getMileage() < 0) {
        throw std::invalid_argument("Mileage cannot be negative\n");
    }

    if (order.getCost() < 0) {
        throw std::invalid_argument("Cost cannot be negative\n");
    }

    if (order.getLoad() < 0) {
        throw std::invalid_argument("Load cannot be negative\n");
    }

    return true;
}

bool Validator::validDispatcher(const Dispatcher &dispatcher) {

    if (dispatcher.getLogin().empty()) {
        throw std::invalid_argument("Login must not be empty\n");
    }

    if (dispatcher.getPassHash().empty()) {
        throw std::invalid_argument("Password must not be empty\n");
    }

    if (!std::regex_match(dispatcher.getName(), name_pattern)) {
        throw std::invalid_argument("Name must consist only of latin characters\n");
    }

    if (!std::regex_match(dispatcher.getSurname(), name_pattern)) {
        throw std::invalid_argument("Surname must consist only of latin characters\n");
    }

    if (!std::regex_match(dispatcher.getAddress(), address_pattern)) {
        throw std::invalid_argument("Address must consist only of latin characters\n");
    }

    if (!std::regex_match(dispatcher.getCity(), city_pattern)) {
        throw std::invalid_argument("City must consist only of latin characters\n");
    }

    return true;
}

bool Validator::validCar(const Car &car, sqlite3 *db) {
    try {
        Driver d;
        d.getDataFromDb(db, car.getDriverId());
    } catch (const std::exception& e) {
        throw std::invalid_argument("There is no driver to own this car\n");
    }

    if (!Validator::validLicense(car.getLicense())) {
        throw std::invalid_argument("Invalid car license");
    }

    if (car.getMileageBuy() < 0) {
        throw std::invalid_argument("Invalid car mileage");
    }

    if (car.getLoadCapacity() < 0) {
        throw std::invalid_argument("Invalid car mileage");
    }

    return true;
}