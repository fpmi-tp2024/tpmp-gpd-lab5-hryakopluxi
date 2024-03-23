//
// Created by hakeyn on 21.3.24.
//

#include "../include/car.h"

void Car::insertCarToDb(sqlite3 *db) {
    char *sql = "INSERT INTO car "
                "(driver_id, license, brand, mileage, load_capacity) VALUES "
                "(?, ?, ?, ?, ?);";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare insert statement: %s\n", sqlite3_errmsg(db));
        throw InternalErrorException("Failed to prepare insert car statement\n");
    }

    sqlite3_bind_int(stmt, 1, driver_id);
    sqlite3_bind_text(stmt, 2, license.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, brand.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 4, mileage_buy);
    sqlite3_bind_double(stmt, 5, load_capacity);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Failed to execute insert statement: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        throw InternalErrorException("Failed to execute statement\n");
    }

    sqlite3_finalize(stmt);
}
