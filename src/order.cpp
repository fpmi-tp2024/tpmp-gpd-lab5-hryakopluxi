//
// Created by Stanislau Senkevich on 21.3.24.
//

#include "../include/order.h"


void Order::getDataFromDb(sqlite3 *db, int order_id) {
    std::string sql = "SELECT * FROM autopark_order WHERE id = ?;";

    sqlite3_stmt *stmt = SQL::prepareSQLStatement(db, sql,
                                    "Failed to prepare select order statement: ");

    sqlite3_bind_int(stmt, 1, order_id);

    stmt = SQL::executeSQLStatement(db, stmt, SQLITE_ROW,
                                    "Failed to execute select order statement: ",
                                    false, false);

    id = sqlite3_column_int(stmt, 0);
    driver_id = sqlite3_column_int(stmt, 1);
    car_id = sqlite3_column_int(stmt, 2);
    date = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));
    mileage = sqlite3_column_double(stmt, 4);
    load = sqlite3_column_double(stmt, 5);
    cost = sqlite3_column_double(stmt, 6);
    is_approved = sqlite3_column_int(stmt, 7);
}

int Order::insertOrderToDb(sqlite3 *db) {
    std::string sql = "INSERT INTO autopark_order (driver_id, car_id, date, mileage, load, cost, is_approved) "
                "VALUES (?, ?, ?, ?, ?, ?, ?);";


    sqlite3_stmt *stmt = SQL::prepareSQLStatement(db, sql,
                                    "Failed to prepare insert order statement: ");

    sqlite3_bind_int(stmt, 1, driver_id);
    sqlite3_bind_int(stmt, 2, car_id);
    sqlite3_bind_text(stmt, 3, date.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 4, mileage);
    sqlite3_bind_double(stmt, 5, load);
    sqlite3_bind_double(stmt, 6, cost);
    sqlite3_bind_int(stmt, 7, is_approved);

    SQL::executeSQLStatement(db, stmt, SQLITE_DONE,
                             "Failed to execute insert order statement: ");
    return static_cast<int>(sqlite3_last_insert_rowid(db));
}

void Order::getDataFromConsole() {
    std::cout << "Enter driver ID: ";
    std::cin >> driver_id;

    std::cout << "Enter car ID: ";
    std::cin >> car_id;

    std::cout << "Enter date (yyyy-mm-dd): ";
    std::cin.ignore();
    std::getline(std::cin, date);

    std::cout << "Enter mileage: ";
    std::cin >> mileage;

    std::cout << "Enter load: ";
    std::cin >> load;

    std::cout << "Enter cost: ";
    std::cin >> cost;
}