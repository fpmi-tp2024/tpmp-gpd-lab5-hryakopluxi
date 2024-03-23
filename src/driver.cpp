//
// Created by hakeyn on 21.3.24.
//
#include "../include/driver.h"
#include "../include/exceptions.h"

std::string Driver::getCategoryString() const {
    std::string str = "";
    for (auto cat: category) {
        switch (cat) {
            case Category::A:
                str += "A ";
                break;
            case Category::B:
                str += "B ";
                break;
            case Category::C:
                str += "C ";
                break;
            case Category::D:
                str += "D ";
                break;
            case Category::BE:
                str += "BE ";
                break;
            case Category::CE:
                str += "CE ";
                break;
        }
    }
    if (str.size() > 0) {
        str.pop_back();
    }

    return str;
}

void Driver::setCategoryFromStr(const std::string &str) {
    category.clear();
    std::istringstream iss(str);
    std::string categoryToken;
    static const std::unordered_map<std::string, Category> categoryMap = {
            {"A",  Category::A},
            {"B",  Category::B},
            {"C",  Category::C},
            {"D",  Category::D},
            {"BE", Category::BE},
            {"CE", Category::CE}
    };

    while (iss >> categoryToken) {
        auto it = categoryMap.find(categoryToken);
        if (it != categoryMap.end()) {
            category.push_back(it->second);
        }
    }
}

void Driver::getDataFromDb(sqlite3 *db, int user_id) {
    char *sql = "SELECT * FROM autopark_driver WHERE user_id = ?";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::string errMsg = "Failed to prepare select driver statement: ";
        errMsg += sqlite3_errmsg(db);
        errMsg += "\n";
        throw InternalErrorException(errMsg);
    }

    sqlite3_bind_int(stmt, 1, user_id);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        std::string errMsg = "Failed to execute select driver statement: ";
        errMsg += sqlite3_errmsg(db);
        errMsg += "\n";
        throw InternalErrorException(errMsg);
    }

    setId( user_id);
    name = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
    surname = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
    setCategoryFromStr(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3)));
    address = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4));
    city = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 5));
    birthday = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 6));
}

void Driver::insertUserToDb(sqlite3 *db) {
    sqlite3_stmt *stmt;
    sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);

    char *sql = "INSERT INTO autopark_user (login, pass_hash, role) VALUES (?, ?, ?);";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::string errMsg = "Failed to prepare insert user statement: ";
        errMsg += sqlite3_errmsg(db);
        errMsg += "\n";
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        throw InternalErrorException(errMsg);
    }

    std::vector<std::string> insert_data{
            getLogin(),
            getPassHash(),
    };
    int role = static_cast<int>(getRole());

    sqlite3_bind_text(stmt, 1, insert_data[0].c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, insert_data[1].c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, role);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::string errMsg = "Failed to execute insert user statement: ";
        errMsg += sqlite3_errmsg(db);
        errMsg += "\n";
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        throw InternalErrorException(errMsg);
    }
    sqlite3_finalize(stmt);

    setId(static_cast<int>(sqlite3_last_insert_rowid(db)));


    sql = "INSERT INTO autopark_driver (user_id, name, surname, category, address, city, birthday) "
          "VALUES (?, ?, ?, ?, ?, ?, ?);";

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::string errMsg = "Failed to prepare insert driver statement: ";
        errMsg += sqlite3_errmsg(db);
        errMsg += "\n";
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        throw InternalErrorException(errMsg);
    }

    insert_data = {
            User::toLower(name),
            User::toLower(surname),
            getCategoryString(),
            User::toLower(address),
            User::toLower(city),
            birthday,
    };

    int id = getId();

    sqlite3_bind_int(stmt, 1, id);
    sqlite3_bind_text(stmt, 2, insert_data[0].c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, insert_data[1].c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, insert_data[2].c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, insert_data[3].c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, insert_data[4].c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 7, insert_data[5].c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::string errMsg = "Failed to execute insert driver statement: ";
        errMsg += sqlite3_errmsg(db);
        errMsg += "\n";
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        throw InternalErrorException(errMsg);
    }

    sqlite3_finalize(stmt);
    sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
}