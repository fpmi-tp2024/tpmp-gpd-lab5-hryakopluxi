//
// Created by Stanislau Senkevich on 21.3.24.
//

#include "../include/config.h"

Config::Config(sqlite3 *db, const std::string& config_name) {
    std::string sql = "SELECT driver_earn_percent "
                      "FROM autopark_config "
                      "WHERE config_name = ?";
    sqlite3_stmt* stmt = SQL::prepareSQLStatement(db, sql,
                                                  "Failed to prepare config statement: ");
    sqlite3_bind_text(stmt, 1, config_name.c_str(), -1, SQLITE_STATIC);
    SQL::executeSQLStatement(db, stmt, SQLITE_ROW,
                             "Failed to execute config statement: ",
                             false, false);
    driverPercent = sqlite3_column_double(stmt, 0);
}