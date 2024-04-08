//
// Created by Stanislau Senkevich on 21.3.24.
//

#include "../include/validator.h"

bool Validator::validName(const std::string& name) {
    return std::regex_match(name, NAME_PATTERN);
}

bool Validator::validSurname(const std::string& surname) {
    return std::regex_match(surname, NAME_PATTERN);
}

bool Validator::validAddress(const std::string& address) {
    return std::regex_match(address, ADDRESS_PATTERN);
}

bool Validator::validCity(const std::string& city) {
    return std::regex_match(city, CITY_PATTERN);
}

bool Validator::validDate(const std::string &date_str) {
    if (!std::regex_match(date_str, DATE_PATTERN)) {
        return false;
    }

    std::tm date = {};
    std::istringstream ss(date_str);
    ss >> date.tm_year;
    ss.ignore();
    ss >> date.tm_mon;
    ss.ignore();
    ss >> date.tm_mday;

    int maxDay;
    switch (date.tm_mon) {
        case 2:
            maxDay = (date.tm_year % 4 == 0 && (date.tm_year % 100 != 0 || date.tm_year % 400 == 0)) ? 29 : 28;
            break;
        case 4:
        case 6:
        case 9:
        case 11:
            maxDay = 30;
            break;
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            maxDay = 31;
            break;
        default:
            return false;
    }
    if (date.tm_mday > maxDay || date.tm_mday < 1 || date.tm_year < 2000) {
        return false;
    }

    return true;
}

bool Validator::validLicense(const std::string &license) {
    return std::regex_match(license, LICENSE_PATTERN);
}

bool Validator::validLogin(const std::string& login) {
    return std::regex_match(login, LOGIN_PATTERN);
}

bool Validator::validPassword(const std::string& password) {
    return std::regex_match(password, PASSWORD_PATTERN);
}

bool Validator::validPeriod(const std::string &date_start, const std::string &date_end) {
    if (!validDate(date_start) || !validDate(date_end)) {
        return false;
    }

    struct std::tm start_tm = {}, end_tm = {};
    std::istringstream start_ss(date_start), end_ss(date_end);
    start_ss >> start_tm.tm_year;
    start_ss.ignore();
    start_ss >> start_tm.tm_mon;
    start_ss.ignore();
    start_ss >> start_tm.tm_mday;

    end_ss >> end_tm.tm_year;
    end_ss.ignore();
    end_ss >> end_tm.tm_mon;
    end_ss.ignore();
    end_ss >> end_tm.tm_mday;

    std::time_t start_time = std::mktime(&start_tm);
    std::time_t end_time = std::mktime(&end_tm);

    return start_time <= end_time;
}

bool Validator::validAge(const std::string &date_str) {
    if (!validDate(date_str)) {
        return false;
    }
    std::tm date = {};
    std::istringstream ss(date_str);
    ss >> date.tm_year;
    ss.ignore();
    ss >> date.tm_mon;
    ss.ignore();
    ss >> date.tm_mday;

    auto now = std::chrono::system_clock::now();
    auto currentTime = std::chrono::system_clock::to_time_t(now);
    std::tm current = *std::localtime(&currentTime);

    current.tm_year -= 18 - 1900;
    return std::mktime(&date) <= std::mktime(&current);
}

bool Validator::validDriver(const Driver &driver) {
    if (!validLogin(driver.getLogin())) {
        throw std::invalid_argument(INVALID_LOGIN_MSG);
    }
    if (!validPassword(driver.getPassHash())) {
        throw std::invalid_argument(INVALID_PASSWORD_MSG);
    }
    if (!validName(driver.getName())) {
        throw std::invalid_argument(INVALID_NAME_MSG);
    }
    if (!validSurname(driver.getSurname())) {
        throw std::invalid_argument(INVALID_SURNAME_MSG);
    }
    if (driver.getExperience() < 0) {
        throw std::invalid_argument(INVALID_EXPERIENCE_MSG);
    }

    if (!validAddress(driver.getAddress())) {
        throw std::invalid_argument(INVALID_ADDRESS_MSG);
    }

    if (!validCity(driver.getCity())) {
        throw std::invalid_argument(INVALID_CITY_MSG);
    }

    if (!validAge(driver.getBirthday())) {
        throw std::invalid_argument(INVALID_AGE_MSG);
    }

    return true;
}

bool Validator::validDispatcher(const Dispatcher &dispatcher) {

    if (!validLogin(dispatcher.getLogin())) {
        throw std::invalid_argument(INVALID_LOGIN_MSG);
    }
    if (!validPassword(dispatcher.getPassHash())) {
        throw std::invalid_argument(INVALID_PASSWORD_MSG);
    }
    if (!validName(dispatcher.getName())) {
        throw std::invalid_argument(INVALID_NAME_MSG);
    }
    if (!validSurname(dispatcher.getSurname())) {
        throw std::invalid_argument(INVALID_SURNAME_MSG);
    }

    if (!validAddress(dispatcher.getAddress())) {
        throw std::invalid_argument(INVALID_ADDRESS_MSG);
    }

    if (!validCity(dispatcher.getCity())) {
        throw std::invalid_argument(INVALID_CITY_MSG);
    }

    return true;
}

bool Validator::validOrder(const Order &order, sqlite3 *db) {
    Driver d;
    Car c;
    try {
        d.getDataFromDb(db, order.getDriverId());
    } catch (const std::exception &e) {
        throw std::invalid_argument(DRIVER_NOT_FOUND_MSG);
    }

    try {
        c.getDataFromDb(db, order.getCarId());
    } catch (const std::exception &e) {
        throw std::invalid_argument(CAR_NOT_FOUND_MSG);
    }

    if (c.getDriverId() != d.getId()) {
        throw std::invalid_argument(DRIVER_NOT_OBTAIN_CAR_MSG);
    }

    if (c.getLoadCapacity() < order.getLoad()) {
        throw std::invalid_argument(LOAD_TOO_BIG_MSG);
    }

    if (!validDate(order.getDate())) {
        throw std::invalid_argument(INVALID_DATE_MSG);
    }

    if (order.getMileage() < 0) {
        throw std::invalid_argument(INVALID_MILEAGE_MSG);
    }

    if (order.getCost() < 0) {
        throw std::invalid_argument(INVALID_COST_MSG);
    }

    if (order.getLoad() < 0) {
        throw std::invalid_argument(INVALID_LOAD_MSG);
    }

    return true;
}

bool Validator::validCar(const Car &car, sqlite3 *db) {
    try {
        Driver d;
        d.getDataFromDb(db, car.getDriverId());
    } catch (const std::exception &e) {
        throw std::invalid_argument(DRIVER_NOT_FOUND_MSG);
    }

    if (!Validator::validLicense(car.getLicense())) {
        throw std::invalid_argument(INVALID_LICENSE_MSG);
    }

    if (car.getMileageBuy() < 0) {
        throw std::invalid_argument(INVALID_MILEAGE_MSG);
    }

    if (car.getLoadCapacity() < 0) {
        throw std::invalid_argument(INVALID_LOAD_CAPACITY_MSG);
    }

    return true;
}

void Validator::validateUpdateCar(Car &update, int car_id, sqlite3 *db) {
    Car old;

    try {
        old.getDataFromDb(db, car_id);
    } catch (const std::exception &e) {
        throw std::invalid_argument(CAR_NOT_FOUND_MSG);
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
        throw std::invalid_argument(DRIVER_NOT_FOUND_MSG);
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
        throw std::invalid_argument(DISPATCHER_NOT_FOUND_MSG);
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
        throw std::invalid_argument(ORDER_NOT_FOUND_MSG);
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
        throw std::invalid_argument(USER_NOT_FOUND_MSG);
    }

    if (update.getLogin().empty()) {
        update.setLogin(old.getLogin());
    }

    if (update.getPassHash().empty()) {
        update.setPassHash(old.getPassHash());
    } else {
        update.setPassHash(hashPassword(update.getPassHash()));
    }
}

std::string Validator::hashPassword(const std::string &password)
{
    /*
        EVP is openssl high-level interface I use for digest(hash) a password
        It requires EVP_DigestInit, EVP_DigestUpdate and EVP_DigestFinal
        The result is put into hashed variable
    */
    EVP_MD_CTX *context = EVP_MD_CTX_new();

    std::string hashed;

    if (context != nullptr)
    {
        if (EVP_DigestInit_ex(context, EVP_sha256(), nullptr))
        {
            if (EVP_DigestUpdate(context, password.c_str(), password.length()))
            {
                unsigned char hash[EVP_MAX_MD_SIZE];
                unsigned int lengthOfHash = 0;

                if (EVP_DigestFinal_ex(context, hash, &lengthOfHash))
                {
                    std::stringstream ss;
                    for (unsigned int i = 0; i < lengthOfHash; ++i)
                    {
                        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
                    }

                    hashed = ss.str();
                }
            }
        }

        EVP_MD_CTX_free(context);
    }
    return hashed;
}

bool Validator::validatePassword(const std::string& hash_pass, const std::string& provided_pass) {
    std::string provided_hash = hashPassword(provided_pass);
    return hash_pass == provided_hash;
}

const std::regex Validator::NAME_PATTERN(R"(^[A-Za-z]{1,}$)");
const std::regex Validator::ADDRESS_PATTERN(R"(^[a-zA-Z]+(?:\s+[a-zA-Z0-9]+)*$)");
const std::regex Validator::CITY_PATTERN(R"(^[A-Za-z]+$)");
const std::regex Validator::DATE_PATTERN(R"(^\d{4}-\d{2}-\d{2}$)");
const std::regex Validator::LICENSE_PATTERN("^(?!0000)[0-9]{4}[ABEIKMHOPCTX]{2}-[1-7]$");
const std::regex Validator::LOGIN_PATTERN("^[A-Za-z0-9_]{1,}$");
const std::regex Validator::PASSWORD_PATTERN("^[A-Za-z0-9_]{1,}$");

const std::string Validator::INVALID_LOGIN_MSG = "Login must contain only latin symbols, digits or _";
const std::string Validator::INVALID_PASSWORD_MSG = "Password must contain only latin symbols, digits or _";
const std::string Validator::INVALID_DATE_MSG = "Invalid date was provided";
const std::string Validator::INVALID_AGE_MSG = "Age must be >= 18 years";
const std::string Validator::INVALID_LICENSE_MSG = "Invalid license was provided";
const std::string Validator::INVALID_ADDRESS_MSG = "Address must contain only latin symbols, digits, or .,/-";
const std::string Validator::INVALID_CITY_MSG = "City must contain only latin symbols";
const std::string Validator::INVALID_NAME_MSG = "Name must contain only latin symbols";
const std::string Validator::INVALID_SURNAME_MSG = "Surname must contain only latin symbols";
const std::string Validator::INVALID_EXPERIENCE_MSG = "Invalid experience was provided";
const std::string Validator::INVALID_LOAD_MSG = "Invalid load was provided";
const std::string Validator::INVALID_LOAD_CAPACITY_MSG = "Invalid load was provided";
const std::string Validator::INVALID_COST_MSG = "Invalid cost was provided";
const std::string Validator::INVALID_MILEAGE_MSG = "Invalid mileage was provided";
const std::string Validator::DRIVER_NOT_FOUND_MSG = "Driver not found";
const std::string Validator::DISPATCHER_NOT_FOUND_MSG = "Dispatcher not found";
const std::string Validator::CAR_NOT_FOUND_MSG = "Car not found";
const std::string Validator::ORDER_NOT_FOUND_MSG = "Order not found";
const std::string Validator::USER_NOT_FOUND_MSG = "User not found";
const std::string Validator::DRIVER_NOT_OBTAIN_CAR_MSG = "The driver doesn't obtain the car";
const std::string Validator::LOAD_TOO_BIG_MSG = "The load is too heavy for this car";