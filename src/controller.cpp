//
// Created by hakeyn on 21.3.24.
//

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
        throw InternalErrorException("Failed to prepare select user statement\n");
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

    if (user->getRole() == DISPATCHER && user->getCity() != driver.getCity()) {
        throw PermissionDeniedException();
    }

    Validator::validDriver(driver);
    driver.setPassHash(BCrypt::generateHash(driver.getPassHash()));

    driver.insertUserToDb(db);
}