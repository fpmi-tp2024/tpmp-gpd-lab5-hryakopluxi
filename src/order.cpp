//
// Created by hakeyn on 21.3.24.
//

#include "../include/order.h"
#include "../include/exceptions.h"


void Order::getDataFromDb(sqlite3 *db, int order_id) {
    char* sql = "SELECT * FROM autopark_order WHERE id = ?;";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::string errMsg =  "Failed to prepare select order statement: ";
        errMsg += sqlite3_errmsg(db);
        errMsg += "\n";
        throw InternalErrorException(errMsg);
    }

    sqlite3_bind_int(stmt, 1, order_id);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        std::string errMsg =  "Failed to execute select order statement: ";
        errMsg += sqlite3_errmsg(db);
        errMsg += "\n";
        throw InternalErrorException(errMsg);
    }

    driver_id = sqlite3_column_int(stmt, 1);
    car_id = sqlite3_column_int(stmt, 2);
    date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
    mileage = sqlite3_column_double(stmt, 4);
    load = sqlite3_column_double(stmt, 5);
    cost = sqlite3_column_double(stmt, 6);
    is_approved = sqlite3_column_int(stmt, 7);
}

void Order::insertOrderToDb(sqlite3 *db) {
    char* sql = "INSERT INTO autopark_order (driver_id, car_id, date, mileage, load, cost, is_approved) "
                "VALUES (?, ?, ?, ?, ?, ?, ?);";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::string errMsg =  "Failed to prepare insert order statement: ";
        errMsg += sqlite3_errmsg(db);
        errMsg += "\n";
        throw InternalErrorException(errMsg);
    }

    sqlite3_bind_int(stmt, 1, driver_id);
    sqlite3_bind_int(stmt, 2, car_id);
    sqlite3_bind_text(stmt, 3, date.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 4, mileage);
    sqlite3_bind_double(stmt, 5, load);
    sqlite3_bind_double(stmt, 6, cost);
    sqlite3_bind_int(stmt, 7, is_approved);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::string errMsg =  "Failed to execute insert order statement: ";
        errMsg += sqlite3_errmsg(db);
        errMsg += "\n";
        throw InternalErrorException(errMsg);
    }
    sqlite3_finalize(stmt);
}