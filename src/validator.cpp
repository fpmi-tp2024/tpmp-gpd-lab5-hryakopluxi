//
// Created by Stanislau Senkevich on 21.3.24.
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

bool Validator::validAge(const std::string& date_str) {
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

bool Validator::validDate(const std::string& date_str) {
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
    } catch (const std::exception &e) {
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

void Validator::validateUpdateCar(Car &update, int car_id, sqlite3 *db) {
    Car old;

    try {
        old.getDataFromDb(db, car_id);
    } catch (const std::exception &e) {
        throw std::invalid_argument("No car found by provided id");
    }

    if (update.getLicense().empty()) {
        update.setLicense(old.getLicense());
    }

    if (update.getBrand().empty()) {
        update.setBrand(old.getBrand());
    }

    if (update.getDriverId() == 0) {
        update.setDriverId(old.getDriverId());
    }

    if (update.getLoadCapacity() == 0) {
        update.setLoadCapacity(old.getLoadCapacity());
    }

    if (update.getMileageBuy() == 0) {
        update.setMileageBuy(old.getMileageBuy());
    }

    Validator::validCar(update, db);
}

void Validator::validateUpdateDriver(Driver &update, int driver_id, sqlite3 *db) {
    Driver old;
    try {
        old.getDataFromDb(db, driver_id);
    } catch (const std::exception &e) {
        throw std::invalid_argument("No driver found by provided id");
    }

    update.setLogin("TEMP");
    update.setPassHash("TEMP");

    if (update.getName().empty()) {
        update.setName(old.getName());
    }

    if (update.getSurname().empty()) {
        update.setSurname(old.getSurname());
    }

    if (update.getCategoryString().empty()) {
        update.setCategory(old.getCategories());
    }

    if (update.getExperience() == 0) {
        update.setExperience(old.getExperience());
    }

    if (update.getAddress().empty()) {
        update.setAddress(old.getAddress());
    }

    if (update.getCity().empty()) {
        update.setCity(old.getCity());
    }

    if (update.getBirthday().empty()) {
        update.setBirthday(old.getBirthday());
    }

    Validator::validDriver(update);
}

void Validator::validateUpdateDispatcher(Dispatcher &update, int dispatcher_id, sqlite3 *db) {
    Dispatcher old;
    try {
        old.getDataFromDb(db, dispatcher_id);
    } catch (const std::exception &e) {
        throw std::invalid_argument("No dispatcher found by provided id");
    }

    update.setLogin("TEMP");
    update.setPassHash("TEMP");

    if (update.getName().empty()) {
        update.setName(old.getName());
    }

    if (update.getSurname().empty()) {
        update.setSurname(old.getSurname());
    }

    if (update.getAddress().empty()) {
        update.setAddress(old.getAddress());
    }

    if (update.getCity().empty()) {
        update.setCity(old.getCity());
    }


    Validator::validDispatcher(update);
}

void Validator::validateUpdateOrder(Order &update, int order_id, sqlite3 *db) {
    Order old;
    try {
        old.getDataFromDb(db, order_id);
    } catch (const std::exception &e) {
        throw std::invalid_argument("No order found by provided id");
    }

    if (update.getDriverId() == 0) {
        update.setDriverId(old.getDriverId());
    }

    if (update.getCarId() == 0) {
        update.setCarId(old.getCarId());
    }

    if (update.getDate().empty()) {
        update.setDate(old.getDate());
    }

    if (update.getMileage() == 0.) {
        update.setMileage(old.getMileage());
    }

    if (update.getLoad() == 0.) {
        update.setLoad(old.getLoad());
    }

    if (update.getCost() == 0.) {
        update.setCost(old.getCost());
    }

    Validator::validOrder(update, db);
}

void Validator::validateUpdateUser(User &update, int user_id, sqlite3 *db) {
    User old;
    try {
        old.getDataFromDb(db, user_id);
    } catch (const std::exception &c) {
        throw std::invalid_argument("No user found by provided id");
    }

    if (update.getLogin().empty()) {
        update.setLogin(old.getLogin());
    }

    if (update.getPassHash().empty()) {
        update.setPassHash(old.getPassHash());
    } else {
        update.setPassHash(BCrypt::generateHash(update.getPassHash()));
    }
}