//
// Created by hakeyn on 21.3.24.
//

#include "../include/controller.h"
#include "../include/exceptions.h"
#include <iostream>

static int callbackLogin(void *NotUsed, int argc, char **argv, char **azColName) {
    for(int i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

Controller::Controller(std::string db_filename) {
    int rc = sqlite3_open(db_filename.c_str(), &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        throw std::runtime_error("Cannot open database");
    }
}

bool Controller::login(std::string login, std::string password) {
    char* sql = "SELECT * FROM user WHERE login = ? ;";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare select statement: %s\n", sqlite3_errmsg(db));
        throw InternalErrorException();
    }

    sqlite3_bind_text(stmt, 1, login.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        std::string table_pass = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        if (!BCrypt::validatePassword(password, table_pass)) {
            return false;
        }
        user.setId(sqlite3_column_int(stmt, 0));
        user.setLogin(login);
        int roleInt = sqlite3_column_int(stmt, 3);
        switch (roleInt) {
            case 1:
                user.setRole(Role::DRIVER);
                break;
            case 2:
                user.setRole(Role::DISPATCHER);
                break;
            case 3:
                user.setRole(Role::ADMIN);
                break;
            default:
                user.setRole(Role::GUEST);
                return false;
        }

        return true;
    }


    return false;
}

void Controller::logout() {
    user.clear();
}

void Controller::addCar(Car &car) {
    if (user.getRole() != ADMIN && user.getRole() != DISPATCHER) {
        throw PermissionDeniedException();
    }

    if (!Validator::validLicense(car.getLicense())) {
        throw InvalidCarLicenseException();
    }

    char* sql = "INSERT INTO car "
                "(driver_id, license, brand, mileage, load_capacity) VALUES "
                "(?, ?, ?, ?, ?);";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare insert statement: %s\n", sqlite3_errmsg(db));
        throw InternalErrorException();
    }

    std::string license = car.getLicense();
    std::string brand = car.getBrand();

    sqlite3_bind_int(stmt, 1, car.getDriverId());
    sqlite3_bind_text(stmt, 2, license.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, brand.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 4, car.getMileageBuy());
    sqlite3_bind_double(stmt, 5, car.getLoadCapacity());

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Failed to execute insert statement: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        throw InternalErrorException();
    }

    sqlite3_finalize(stmt);
}