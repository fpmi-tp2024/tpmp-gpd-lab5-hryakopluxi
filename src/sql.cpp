//
// Created by Stanislau Senkevich on 24.3.24.
//

#include "../include/sql.h"

sqlite3_stmt *SQL::prepareSQLStatement(sqlite3 *db, const std::string &sql, sqlite3_stmt *stmt,
                                       int required_status, std::string errMsg, bool rollback) {
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != required_status) {
        errMsg += sqlite3_errmsg(db);
        errMsg += "\n";
        if (rollback) {
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        }
        throw InternalErrorException(errMsg);
    }

    return stmt;
}

sqlite3_stmt *SQL::executeSQLStatement(sqlite3 *db, sqlite3_stmt *stmt, int required_status,
                                       std::string errMsg, bool rollback, bool finalize) {
    int rc = sqlite3_step(stmt);
    if (rc != required_status) {
        errMsg += sqlite3_errmsg(db);
        errMsg += "\n";
        if (rollback) {
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        }
        throw InternalErrorException(errMsg);
    }
    if (finalize) {
        sqlite3_finalize(stmt);
    }
    return stmt;
}
