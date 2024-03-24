//
// Created by hakeyn on 21.3.24.
//

#include "../include/user.h"

std::string User::toLower(std::string str) {
    for (char & i : str) {
        i = std::tolower(i);
    }
    return str;
}

void User::getDataFromDb(sqlite3 *db, int user_id) {
    char* sql = "SELECT * FROM autopark_user WHERE id = ?;";
    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::string errMsg = "Failed to prepare select user statement: ";
        errMsg += sqlite3_errmsg(db);
        errMsg += "\n";
        throw InternalErrorException(errMsg);
    }

    sqlite3_bind_int(stmt, 1, user_id);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        std::string errMsg = "Failed to execute select user statement: ";
        errMsg += sqlite3_errmsg(db);
        errMsg += "\n";
        throw InternalErrorException(errMsg);
    }

    id = user_id;
    login = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
    pass_hash = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
    role = static_cast<Role>(sqlite3_column_int(stmt, 3));
}