//
// Created by hakeyn on 21.3.24.
//

#include "../include/car.h"

void Car::getDataFromDb(sqlite3 *db, int car_id) {
    char* sql = "SELECT * FROM autopark_car WHERE id = ?;";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::string errMsg =  "Failed to prepare select car statement: ";
        errMsg += sqlite3_errmsg(db);
        errMsg += "\n";
        throw InternalErrorException(errMsg);
    }

    sqlite3_bind_int(stmt, 1, car_id);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        std::string errMsg =  "Failed to execute select car statement: ";
        errMsg += sqlite3_errmsg(db);
        errMsg += "\n";
        throw InternalErrorException(errMsg);
    }

    driver_id = sqlite3_column_int(stmt, 1);
    license = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
    brand = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
    mileage_buy = sqlite3_column_int(stmt, 4);
    load_capacity = sqlite3_column_int(stmt, 5);
}

void Car::insertCarToDb(sqlite3 *db) {
    char *sql = "INSERT INTO autopark_car "
                "(driver_id, license, brand, mileage, load_capacity) VALUES "
                "(?, ?, ?, ?, ?);";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::string errMsg =  "Failed to prepare insert car statement: ";
        errMsg += sqlite3_errmsg(db);
        errMsg += "\n";
        throw InternalErrorException(errMsg);
    }

    sqlite3_bind_int(stmt, 1, driver_id);
    sqlite3_bind_text(stmt, 2, license.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, brand.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 4, mileage_buy);
    sqlite3_bind_double(stmt, 5, load_capacity);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::string errMsg =  "Failed to execute insert car statement: ";
        errMsg += sqlite3_errmsg(db);
        errMsg += "\n";
        throw InternalErrorException(errMsg);
    }

    sqlite3_finalize(stmt);
}
