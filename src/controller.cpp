//
// Created by hakeyn on 21.3.24.
//

#include <iostream>
#include "../include/controller.h"

Controller::Controller(const std::string& db_filename) {
    int rc = sqlite3_open(db_filename.c_str(), &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        throw std::runtime_error("Cannot open database");
    }
}

bool Controller::login(const std::string& login, const std::string& password) {
    char *sql = "SELECT * FROM autopark_user WHERE login = ? ;";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::string errMsg =  "Failed to prepare select user statement: ";
        errMsg += sqlite3_errmsg(db);
        errMsg += "\n";
        throw InternalErrorException(errMsg);
    }

    sqlite3_bind_text(stmt, 1, login.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);

    if (rc != SQLITE_ROW) {
        return false;
    }

    std::string table_pass_hash = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
    if (!BCrypt::validatePassword(password, table_pass_hash)) {
        return false;
    }

    int roleInt = sqlite3_column_int(stmt, 3);

    switch (roleInt) {
        case 1:
            user = new Driver();
            user->setRole(Role::DRIVER);
            break;
        case 2:
            user = new Dispatcher();
            user->setRole(Role::DISPATCHER);
            break;
        case 3:
            user = new User();
            user->setRole(Role::ADMIN);
            break;
        default:
            throw InternalErrorException("Failed to get valid data about user\n");
    }
    user->setId(sqlite3_column_int(stmt, 0));
    user->setLogin(login);
    user->getDataFromDb(db, user->getId());

    return true;
}

void Controller::logout() {
    user = nullptr;
}

void Controller::addCar(Car &car) {
    if (user->getRole() != ADMIN && user->getRole() != DISPATCHER) {
        throw PermissionDeniedException();
    }

    Validator::validCar(car, db);

    car.insertCarToDb(db);
}

void Controller::addDriver(Driver &driver) {
    if (user->getRole() != ADMIN && user->getRole() != DISPATCHER) {
        throw PermissionDeniedException();
    }

    if (user->getRole() == DISPATCHER) {
        Dispatcher dispatcher;
        dispatcher.getDataFromDb(db, user->getId());
        if (User::toLower(dispatcher.getCity()) != User::toLower(driver.getCity())) {
            throw PermissionDeniedException();
        }
    }

    Validator::validDriver(driver);
    driver.setPassHash(BCrypt::generateHash(driver.getPassHash()));

    driver.insertUserToDb(db);
}

void Controller::addOrder(Order &order) {
    if (user->getRole() != ADMIN && user->getRole() != DISPATCHER && order.getDriverId() != user->getId()) {
        throw PermissionDeniedException();
    }

    if (user->getRole() == DISPATCHER) {
        Driver driver;
        driver.getDataFromDb(db, order.getDriverId());
        if (driver.getCity() != user->getCity()) {
            throw PermissionDeniedException();
        }
    }

    if (user->getRole() == ADMIN || user->getRole() == DISPATCHER) {
        order.setIsApproved(true);
    } else {
        order.setIsApproved(false);
    }

    try {
        Validator::validOrder(order, db);
    } catch (const std::exception& e) {
        std::string msg = "Invalid order: ";
        msg += e.what();
        throw std::invalid_argument(msg);
    }

    order.insertOrderToDb(db);
}

void Controller::addDispatcher(Dispatcher &dispatcher) {
    if (user->getRole() != ADMIN) {
        throw PermissionDeniedException();
    }

    Validator::validDispatcher(dispatcher);
    dispatcher.setPassHash(BCrypt::generateHash(dispatcher.getPassHash()));

    dispatcher.insertUserToDb(db);
}

void Controller::deleteCar(int car_id) {
    if (user->getRole() != ADMIN) {
        throw PermissionDeniedException();
    }

    char* sql = "DELETE FROM autopark_car WHERE id = ?";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::string errMsg =  "Failed to prepare delete car statement: ";
        errMsg += sqlite3_errmsg(db);
        errMsg += "\n";
        throw InternalErrorException(errMsg);
    }

    sqlite3_bind_int(stmt, 1, car_id);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::string errMsg =  "Failed to execute delete car statement: ";
        errMsg += sqlite3_errmsg(db);
        errMsg += "\n";
        throw InternalErrorException(errMsg);
    }

    sqlite3_finalize(stmt);
}

void Controller::deleteDriver(int user_id) {
    if (user->getRole() != ADMIN) {
        throw PermissionDeniedException();
    }

    try {
        Driver d;
        d.getDataFromDb(db, user_id);
    } catch (const std::exception& e) {
        throw std::invalid_argument("No driver with provided id\n");
    }

    sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);

    char* sql = "DELETE FROM autopark_driver WHERE user_id = ?";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::string errMsg =  "Failed to prepare delete driver statement: ";
        errMsg += sqlite3_errmsg(db);
        errMsg += "\n";
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        throw InternalErrorException(errMsg);
    }

    sqlite3_bind_int(stmt, 1, user_id);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::string errMsg =  "Failed to execute delete driver statement: ";
        errMsg += sqlite3_errmsg(db);
        errMsg += "\n";
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        throw InternalErrorException(errMsg);
    }

    sqlite3_finalize(stmt);

    sql = "DELETE FROM autopark_user WHERE id = ?";

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::string errMsg =  "Failed to prepare delete user statement: ";
        errMsg += sqlite3_errmsg(db);
        errMsg += "\n";
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        throw InternalErrorException(errMsg);
    }

    sqlite3_bind_int(stmt, 1, user_id);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::string errMsg =  "Failed to execute delete user statement: ";
        errMsg += sqlite3_errmsg(db);
        errMsg += "\n";
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        throw InternalErrorException(errMsg);
    }

    sqlite3_finalize(stmt);
    sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
}

void Controller::deleteDispatcher(int user_id) {
    if (user->getRole() != ADMIN) {
        throw PermissionDeniedException();
    }

    try {
        Dispatcher d;
        d.getDataFromDb(db, user_id);
    } catch (const std::exception& e) {
        throw std::invalid_argument("No dispatcher with provided id\n");
    }

    sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);

    char* sql = "DELETE FROM autopark_dispatcher WHERE user_id = ?";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::string errMsg =  "Failed to prepare delete dispatcher statement: ";
        errMsg += sqlite3_errmsg(db);
        errMsg += "\n";
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        throw InternalErrorException(errMsg);
    }

    sqlite3_bind_int(stmt, 1, user_id);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::string errMsg =  "Failed to execute delete dispatcher statement: ";
        errMsg += sqlite3_errmsg(db);
        errMsg += "\n";
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        throw InternalErrorException(errMsg);
    }

    sqlite3_finalize(stmt);

    sql = "DELETE FROM autopark_user WHERE id = ?";

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::string errMsg =  "Failed to prepare delete user statement: ";
        errMsg += sqlite3_errmsg(db);
        errMsg += "\n";
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        throw InternalErrorException(errMsg);
    }

    sqlite3_bind_int(stmt, 1, user_id);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::string errMsg =  "Failed to execute delete user statement: ";
        errMsg += sqlite3_errmsg(db);
        errMsg += "\n";
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        throw InternalErrorException(errMsg);
    }

    sqlite3_finalize(stmt);
    sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
}

void Controller::deleteOrder(int order_id) {
    if (user->getRole() != ADMIN) {
        throw PermissionDeniedException();
    }

    try {
        Order ord;
        ord.getDataFromDb(db, order_id);
    } catch (const std::exception& e) {
        throw std::invalid_argument("No order with provided id\n");
    }

    char* sql = "DELETE FROM autopark_order WHERE id = ?";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::string errMsg =  "Failed to prepare delete order statement: ";
        errMsg += sqlite3_errmsg(db);
        errMsg += "\n";
        throw InternalErrorException(errMsg);
    }

    sqlite3_bind_int(stmt, 1, order_id);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::string errMsg =  "Failed to execute delete order statement: ";
        errMsg += sqlite3_errmsg(db);
        errMsg += "\n";
        throw InternalErrorException(errMsg);
    }

    sqlite3_finalize(stmt);
}

void Controller::updateCar(int car_id, Car& update) {
    if (user->getRole() != ADMIN && user->getRole() != DISPATCHER) {
        throw PermissionDeniedException();
    }

    Validator::validateUpdateCar(update, car_id, db);

    char* sql = "UPDATE autopark_car SET driver_id = ?, license = ?, brand = ?, "
                "mileage = ?, load_capacity = ? "
                "WHERE id = ?;";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::string errMsg =  "Failed to prepare update car statement: ";
        errMsg += sqlite3_errmsg(db);
        errMsg += "\n";
        throw InternalErrorException(errMsg);
    }

    int driver_id = update.getDriverId();
    std::string license = update.getLicense();
    std::string brand = update.getBrand();
    double mileage = update.getMileageBuy();
    double load_capacity = update.getLoadCapacity();

    sqlite3_bind_int(stmt, 1, driver_id);
    sqlite3_bind_text(stmt, 2, license.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, brand.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 4, mileage);
    sqlite3_bind_double(stmt, 5, load_capacity);
    sqlite3_bind_int(stmt, 6, car_id);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::string errMsg =  "Failed to execute update car statement: ";
        errMsg += sqlite3_errmsg(db);
        errMsg += "\n";
        throw InternalErrorException(errMsg);
    }

    sqlite3_finalize(stmt);
}

void Controller::updateDriver(int user_id, Driver& update) {
    if (user->getRole() != ADMIN && user->getRole() != DISPATCHER) {
        throw PermissionDeniedException();
    }

    if (user->getRole() == DISPATCHER) {
        Dispatcher disp;
        disp.getDataFromDb(db, user->getId());
        Driver old;
        try {
            old.getDataFromDb(db, user_id);
        } catch (const std::exception& e) {
            throw std::invalid_argument("No driver found by provided id");
        }
        if (User::toLower(old.getCity()) != User::toLower(disp.getCity())) {
            throw PermissionDeniedException();
        }
    }

    Validator::validateUpdateDriver(update, user_id, db);

    char* sql = "UPDATE autopark_driver SET name = ?, surname = ?, "
                "category = ?, experience = ?, "
                "address = ?, city = ?, birthday = ? "
                "WHERE user_id = ?;";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::string errMsg =  "Failed to prepare update car statement: ";
        errMsg += sqlite3_errmsg(db);
        errMsg += "\n";
        throw InternalErrorException(errMsg);
    }

    int id = user_id;
    std::string name = update.getName();
    std::string surname = update.getSurname();
    std::string category = update.getCategoryString();
    int experience = update.getExperience();
    std::string address = update.getAddress();
    std::string city = update.getCity();
    std::string birthday = update.getBirthday();

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, surname.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, category.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, experience);
    sqlite3_bind_text(stmt, 5, address.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, city.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 7, birthday.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 8, id);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::string errMsg =  "Failed to execute update car statement: ";
        errMsg += sqlite3_errmsg(db);
        errMsg += "\n";
        throw InternalErrorException(errMsg);
    }

    sqlite3_finalize(stmt);
}