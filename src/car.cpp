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
    mileage_purchase = sqlite3_column_int(stmt, 4);
    load_capacity = sqlite3_column_int(stmt, 5);
}

int Car::insertCarToDb(sqlite3 *db) {
    std::string sql = "INSERT INTO autopark_car "
                "(driver_id, license, brand, mileage, load_capacity) VALUES "
                "(?, ?, ?, ?, ?);";

    sqlite3_stmt *stmt = SQL::prepareSQLStatement(db, sql,"Failed to prepare insert car statement: ");

    sqlite3_bind_int(stmt, 1, driver_id);
    sqlite3_bind_text(stmt, 2, license.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, brand.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 4, mileage_purchase);
    sqlite3_bind_double(stmt, 5, load_capacity);

    SQL::executeSQLStatement(db, stmt, SQLITE_DONE,
                             "Failed to execute insert car statement: ");

    return static_cast<int>(sqlite3_last_insert_rowid(db));
}

void Car::getDataFromConsole() {
    std::string dID, mile, load;

    std::cout << "Enter Driver ID: ";
    std::getline(std::cin, dID, '\n');
    std::cout << "Enter license: ";
    std::getline(std::cin, license, '\n');
    std::cout << "Enter brand: ";
    std::getline(std::cin, brand, '\n');
    std::cout << "Enter mileage on purchase: ";
    std::getline(std::cin, mile, '\n');
    std::cout << "Enter load capacity: ";
    std::getline(std::cin, load, '\n');

    driver_id = std::stoi(dID);
    mileage_purchase = std::stod(mile);
    load_capacity = std::stod(load);
}

std::string Car::print() const {
    std::ostringstream oss;
        oss << "\nCar: \n"
            << "\tId: " << id << "\n"
            << "\tDriver id: " << driver_id << "\n"
            << "\tLicense: " << license << "\n"
            << "\tMileage: " << mileage_purchase << "\n"
            << "\tLoad capacity: " << load_capacity << "\n";
        return oss.str();
}