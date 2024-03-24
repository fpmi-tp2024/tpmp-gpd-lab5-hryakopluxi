//
// Created by Stanislau Senkevich on 24.3.24.
//

#ifndef LAB_5_PROJECT_SQL_H
#define LAB_5_PROJECT_SQL_H

#include <sqlite3.h>
#include <string>
#include "exceptions.h"

class SQL {
public:
    static sqlite3_stmt *prepareSQLStatement(sqlite3 *db, const std::string& sql, sqlite3_stmt *stmt,
                                             int required_status, std::string errMsg, bool rollback = false);
    static sqlite3_stmt *executeSQLStatement(sqlite3 *db, sqlite3_stmt *stmt, int required_status,
                                    std::string errMsg, bool rollback = false, bool finalize = true);
};

#endif //LAB_5_PROJECT_SQL_H
