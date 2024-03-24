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

    if (!Validator::validLicense(car.getLicense())) {
        throw std::invalid_argument("Invalid car license");
    }

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