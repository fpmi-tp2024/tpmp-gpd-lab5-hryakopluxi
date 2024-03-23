//
// Created by hakeyn on 21.3.24.
//

#include <sqlite3.h>
#include "../include/dispatcher.h"
#include "../include/exceptions.h"

void Dispatcher::getDataFromDb(sqlite3* db, int user_id) {
    char* sql = "SELECT * FROM autopark_dispatcher WHERE user_id = ?";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::string errMsg =  "Failed to prepare select dispatcher statement: ";
        errMsg += sqlite3_errmsg(db);
        errMsg += "\n";
        throw InternalErrorException(errMsg);
    }

    sqlite3_bind_int(stmt, 1, user_id);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        std::string errMsg =  "Failed to execute select dispatcher statement: ";
        errMsg += sqlite3_errmsg(db);
        errMsg += "\n";
        throw InternalErrorException(errMsg);
    }

    name = (( reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))));
    surname = (( reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2))));
    address = (( reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3))));
    city = (( reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4))));
}