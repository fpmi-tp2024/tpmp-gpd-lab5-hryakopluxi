//
// Created by Stanislau Senkevich on 21.3.24.
//
#include "../include/driver.h"

std::string Driver::getCategoryString() const {
    std::string str;
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
    if (!str.empty()) {
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
    std::string sql = "SELECT * FROM autopark_driver WHERE user_id = ?";

    sqlite3_stmt *stmt = SQL::prepareSQLStatement(db, sql,
                                    "Failed to prepare select driver statement: ");

    sqlite3_bind_int(stmt, 1, user_id);

    stmt = SQL::executeSQLStatement(db, stmt, SQLITE_ROW,
                                    "Failed to execute select driver statement: ",
                                    false, false);

    setId(user_id);
    name = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
    surname = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
    setCategoryFromStr(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3)));
    experience = sqlite3_column_int(stmt, 4);
    address = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 5));
    city = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 6));
    birthday = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 7));
}

int Driver::insertUserToDb(sqlite3 *db) {

    sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);

    std::string sql = "INSERT INTO autopark_user (login, pass_hash, role) VALUES (?, ?, ?);";
    sqlite3_stmt *stmt = SQL::prepareSQLStatement(db, sql,
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
                             "Failed to execute insert user statement: ", true);
    setId(static_cast<int>(sqlite3_last_insert_rowid(db)));

    sql = "INSERT INTO autopark_driver (user_id, name, surname, category, experience, address, city, birthday) "
          "VALUES (?, ?, ?, ?, ?, ?, ?, ?);";

    stmt = SQL::prepareSQLStatement(db, sql,
                                    "Failed to prepare insert driver statement: ", true);

    insert_data = {
            User::toLower(name),
            User::toLower(surname),
            getCategoryString(),
            User::toLower(address),
            User::toLower(city),
            birthday,
    };

    int id = getId();
    int exp = getExperience();

    sqlite3_bind_int(stmt, 1, id);
    sqlite3_bind_text(stmt, 2, insert_data[0].c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, insert_data[1].c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, insert_data[2].c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 5, exp);
    sqlite3_bind_text(stmt, 6, insert_data[3].c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 7, insert_data[4].c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 8, insert_data[5].c_str(), -1, SQLITE_STATIC);

    SQL::executeSQLStatement(db, stmt, SQLITE_DONE,
                             "Failed to execute insert driver statement: ", true);
    sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);

    return getId();
}

void Driver::getDataFromConsole() {
    User::getDataFromConsole(); // Collect data for the base class User

    std::cout << "Enter name: ";
    std::getline(std::cin, name, '\n');

    std::cout << "Enter surname: ";
    std::getline(std::cin, surname, '\n');

    std::cout << "Enter categories (separated by space): ";
    std::cin.ignore();
    std::string categoriesInput;
    std::getline(std::cin, categoriesInput, '\n');
    if (!categoriesInput.empty()) {
        setCategoryFromStr(categoriesInput);
    }


    std::cout << "Enter experience (years): ";
    std::cin >> experience;

    std::cout << "Enter address: ";
    std::cin.ignore();
    std::getline(std::cin, address);

    std::cout << "Enter city: ";
    std::getline(std::cin, city);

    std::cout << "Enter birthday (yyyy-mm-dd): ";
    std::getline(std::cin, birthday, '\n');
}