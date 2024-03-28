//
// Created by Stanislau Senkevich on 21.3.24.
//

#include "../include/car.h"

void Car::getDataFromDb(sqlite3 *db, int car_id) {
    std::string sql = "SELECT * FROM autopark_car WHERE id = ?;";

    sqlite3_stmt *stmt = SQL::prepareSQLStatement(db, sql,"Failed to prepare select car statement: ");

    sqlite3_bind_int(stmt, 1, car_id);

    stmt = SQL::executeSQLStatement(db, stmt, SQLITE_ROW,
                                    "Failed to execute select car statement: ",
                                    false, false);

    id = sqlite3_column_int(stmt, 0);
    driver_id = sqlite3_column_int(stmt, 1);
    license = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
    brand = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));
    mileage_buy = sqlite3_column_int(stmt, 4);
    load_capacity = sqlite3_column_int(stmt, 5);
}

void Car::insertCarToDb(sqlite3 *db) {
    std::string sql = "INSERT INTO autopark_car "
                "(driver_id, license, brand, mileage, load_capacity) VALUES "
                "(?, ?, ?, ?, ?);";

    sqlite3_stmt *stmt = SQL::prepareSQLStatement(db, sql,"Failed to prepare insert car statement: ");

    sqlite3_bind_int(stmt, 1, driver_id);
    sqlite3_bind_text(stmt, 2, license.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, brand.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 4, mileage_buy);
    sqlite3_bind_double(stmt, 5, load_capacity);

    SQL::executeSQLStatement(db, stmt, SQLITE_DONE,
                             "Failed to execute insert car statement: ");
}
