//
// Created by hakeyn on 21.3.24.
//

#include <sqlite3.h>
#include "../include/dispatcher.h"
#include "../include/exceptions.h"

void Dispatcher::getDataFromDb(sqlite3* db, int user_id) {
    char* sql = "SELECT * FROM autopark_dispatcher WHERE user_id = ?";
    sqlite3_stmt *stmt;
    stmt = SQL::prepareSQLStatement(db, sql, stmt, SQLITE_OK,
                                    "Failed to prepare select dispatcher statement: ");

    sqlite3_bind_int(stmt, 1, user_id);

    stmt = SQL::executeSQLStatement(db, stmt, SQLITE_ROW,
                                    "Failed to execute select dispatcher statement: ",
                                    false, false);

    setId(user_id);
    name = (( reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))));
    surname = (( reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2))));
    address = (( reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3))));
    city = (( reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4))));
}

void Dispatcher::insertUserToDb(sqlite3* db) {
    sqlite3_stmt *stmt;
    sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);

    char *sql = "INSERT INTO autopark_user (login, pass_hash, role) VALUES (?, ?, ?);";

    stmt = SQL::prepareSQLStatement(db, sql, stmt, SQLITE_OK,
                                    "Failed to prepare insert user statement: ", true);

    std::vector<std::string> insert_data{
            getLogin(),
            getPassHash(),
    };
    int role = static_cast<int>(getRole());

    sqlite3_bind_text(stmt, 1, insert_data[0].c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, insert_data[1].c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, role);

    SQL::executeSQLStatement(db, stmt, SQLITE_DONE,
                                    "Failed to execute insert user statement: ",true);

    setId(static_cast<int>(sqlite3_last_insert_rowid(db)));

    sql = "INSERT INTO autopark_dispatcher (user_id, name, surname, address, city) "
          "VALUES (?, ?, ?, ?, ?);";

    stmt = SQL::prepareSQLStatement(db, sql, stmt, SQLITE_OK,
                                    "Failed to prepare insert dispatcher statement: ", true);

    insert_data = {
            User::toLower(name),
            User::toLower(surname),
            User::toLower(address),
            User::toLower(city),
    };

    int id = getId();

    sqlite3_bind_int(stmt, 1, id);
    sqlite3_bind_text(stmt, 2, insert_data[0].c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, insert_data[1].c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, insert_data[2].c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, insert_data[3].c_str(), -1, SQLITE_STATIC);

    SQL::executeSQLStatement(db, stmt, SQLITE_DONE,
                                    "Failed to execute insert dispatcher statement: ",true);

    sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
}