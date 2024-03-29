//
// Created by Stanislau Senkevich on 21.3.24.
//

#include "../include/user.h"

std::string User::toLower(std::string str) {
    for (char &i: str) {
        i = std::tolower(i);
    }
    return str;
}

void User::getDataFromDb(sqlite3 *db, int user_id) {
    std::string sql = "SELECT * FROM autopark_user WHERE id = ?;";


    sqlite3_stmt *stmt = SQL::prepareSQLStatement(db, sql,
                                    "Failed to prepare select user statement: ");

    sqlite3_bind_int(stmt, 1, user_id);
    stmt = SQL::executeSQLStatement(db, stmt, SQLITE_ROW,
                                    "Failed to execute select order statement: ",
                                    false, false);

    id = user_id;
    login = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
    pass_hash = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
    role = static_cast<Role>(sqlite3_column_int(stmt, 3));
}

void User::getDataFromConsole() {
    std::cin.ignore();
    std::cout << "Enter login: ";
    getline(std::cin, login, '\n');

    std::cout << "Enter password: ";
    getline(std::cin, pass_hash, '\n');
}