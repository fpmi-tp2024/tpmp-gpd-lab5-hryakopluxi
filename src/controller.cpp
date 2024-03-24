//
// Created by hakeyn on 21.3.24.
//

#include <iostream>
#include "../include/controller.h"

sqlite3_stmt * prepareSQLStatement(sqlite3 *db, char *sql, sqlite3_stmt *stmt, int required_status, std::string errMsg,
                         bool need_rollback) {
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != required_status) {
        errMsg += sqlite3_errmsg(db);
        errMsg += "\n";
        if (need_rollback) {
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        }
        throw InternalErrorException(errMsg);
    }

    return stmt;
}

void executeSQLStatement(sqlite3 *db, sqlite3_stmt *stmt, int required_status, std::string errMsg, bool need_rollback) {
    int rc = sqlite3_step(stmt);
    if (rc != required_status) {
        errMsg += sqlite3_errmsg(db);
        errMsg += "\n";
        if (need_rollback) {
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        }
        throw InternalErrorException(errMsg);
    }
    sqlite3_finalize(stmt);
}

Controller::Controller(const std::string &db_filename) {
    int rc = sqlite3_open(db_filename.c_str(), &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        throw std::runtime_error("Cannot open database");
    }
}

bool Controller::login(const std::string &login, const std::string &password) {
    char *sql = "SELECT * FROM autopark_user WHERE login = ? ;";
    sqlite3_stmt *stmt;
    stmt = prepareSQLStatement(db, sql, stmt, SQLITE_OK, "Failed to prepare login statement: ", false);

    sqlite3_bind_text(stmt, 1, login.c_str(), -1, SQLITE_STATIC);

    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        return false;
    }

    std::string table_pass_hash = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
    if (!BCrypt::validatePassword(password, table_pass_hash)) {
        return false;
    }

    user.setId(sqlite3_column_int(stmt, 0));
    user.setLogin(login);
    return true;
}

void Controller::logout() {
    user = User();
}

void Controller::addCar(Car &car) {
    if (user.getRole() != ADMIN && user.getRole() != DISPATCHER) {
        throw PermissionDeniedException();
    }

    Validator::validCar(car, db);

    car.insertCarToDb(db);
}

void Controller::addDriver(Driver &driver) {
    if (user.getRole() != ADMIN && user.getRole() != DISPATCHER) {
        throw PermissionDeniedException();
    }

    if (user.getRole() == DISPATCHER) {
        Dispatcher dispatcher;
        dispatcher.getDataFromDb(db, user.getId());
        if (User::toLower(dispatcher.getCity()) != User::toLower(driver.getCity())) {
            throw PermissionDeniedException();
        }
    }

    Validator::validDriver(driver);
    driver.setPassHash(BCrypt::generateHash(driver.getPassHash()));

    driver.insertUserToDb(db);
}

void Controller::addOrder(Order &order) {
    if (user.getRole() != ADMIN && user.getRole() != DISPATCHER && order.getDriverId() != user.getId()) {
        throw PermissionDeniedException();
    }

    if (user.getRole() == DISPATCHER) {
        Driver driver;
        Dispatcher disp;
        disp.getDataFromDb(db, user.getId());
        driver.getDataFromDb(db, order.getDriverId());
        if (driver.getCity() != disp.getCity()) {
            throw PermissionDeniedException();
        }
    }

    if (user.getRole() == ADMIN || user.getRole() == DISPATCHER) {
        order.setIsApproved(true);
    } else {
        order.setIsApproved(false);
    }

    try {
        Validator::validOrder(order, db);
    } catch (const std::exception &e) {
        std::string msg = "Invalid order: ";
        msg += e.what();
        throw std::invalid_argument(msg);
    }

    order.insertOrderToDb(db);
}

void Controller::addDispatcher(Dispatcher &dispatcher) {
    if (user.getRole() != ADMIN) {
        throw PermissionDeniedException();
    }

    Validator::validDispatcher(dispatcher);
    dispatcher.setPassHash(BCrypt::generateHash(dispatcher.getPassHash()));

    dispatcher.insertUserToDb(db);
}

void Controller::deleteCar(int car_id) {
    if (user.getRole() != ADMIN) {
        throw PermissionDeniedException();
    }

    char *sql = "DELETE FROM autopark_car WHERE id = ?";
    sqlite3_stmt *stmt;
    stmt = prepareSQLStatement(db, sql, stmt, SQLITE_OK,
                        "Failed to prepare delete car statement: ", false);


    sqlite3_bind_int(stmt, 1, car_id);
    executeSQLStatement(db, stmt, SQLITE_DONE,
                        "Failed to execute delete car statement: ", false);
}

void Controller::deleteDriver(int user_id) {
    if (user.getRole() != ADMIN) {
        throw PermissionDeniedException();
    }

    try {
        Driver d;
        d.getDataFromDb(db, user_id);
    } catch (const std::exception &e) {
        throw std::invalid_argument("No driver with provided id\n");
    }

    sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);

    char *sql = "DELETE FROM autopark_driver WHERE user_id = ?";
    sqlite3_stmt *stmt;
    stmt = prepareSQLStatement(db, sql, stmt, SQLITE_OK,
                        "Failed to prepare delete driver statement: ", true);


    sqlite3_bind_int(stmt, 1, user_id);
    executeSQLStatement(db, stmt, SQLITE_DONE,
                        "Failed to execute delete driver statement: ", true);

    sql = "DELETE FROM autopark_user WHERE id = ?";
    stmt = prepareSQLStatement(db, sql, stmt, SQLITE_OK,
                        "Failed to prepare delete user statement: ", true);


    sqlite3_bind_int(stmt, 1, user_id);
    executeSQLStatement(db, stmt, SQLITE_DONE,
                        "Failed to execute delete user statement: ", true);

    sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
}

void Controller::deleteDispatcher(int user_id) {
    if (user.getRole() != ADMIN) {
        throw PermissionDeniedException();
    }

    try {
        Dispatcher d;
        d.getDataFromDb(db, user_id);
    } catch (const std::exception &e) {
        throw std::invalid_argument("No dispatcher with provided id\n");
    }

    sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);

    char *sql = "DELETE FROM autopark_dispatcher WHERE user_id = ?";
    sqlite3_stmt *stmt;
    stmt = prepareSQLStatement(db, sql, stmt, SQLITE_OK,
                        "Failed to prepare delete dispatcher statement: ", true);

    sqlite3_bind_int(stmt, 1, user_id);
    executeSQLStatement(db, stmt, SQLITE_DONE,
                        "Failed to execute delete dispatcher statement: ", true);

    sql = "DELETE FROM autopark_user WHERE id = ?";
    stmt = prepareSQLStatement(db, sql, stmt, SQLITE_OK,
                        "Failed to prepare delete user statement: ", true);

    sqlite3_bind_int(stmt, 1, user_id);
    executeSQLStatement(db, stmt, SQLITE_DONE,
                        "Failed to execute delete user statement: ", true);
    sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
}

void Controller::deleteOrder(int order_id) {
    if (user.getRole() != ADMIN) {
        throw PermissionDeniedException();
    }

    try {
        Order ord;
        ord.getDataFromDb(db, order_id);
    } catch (const std::exception &e) {
        throw std::invalid_argument("No order with provided id\n");
    }

    char *sql = "DELETE FROM autopark_order WHERE id = ?";
    sqlite3_stmt *stmt;
    stmt = prepareSQLStatement(db, sql, stmt, SQLITE_OK,
                        "Failed to prepare delete order statement: ", false);

    sqlite3_bind_int(stmt, 1, order_id);
    executeSQLStatement(db, stmt, SQLITE_DONE,
                        "Failed to execute delete order statement: ", false);
}

void Controller::updateCar(int car_id, Car &update) {
    if (user.getRole() != ADMIN && user.getRole() != DISPATCHER) {
        throw PermissionDeniedException();
    }

    Validator::validateUpdateCar(update, car_id, db);

    char *sql = "UPDATE autopark_car SET "
                "driver_id = ?, license = ?, brand = ?, "
                "mileage = ?, load_capacity = ? "
                "WHERE id = ?;";

    sqlite3_stmt *stmt;
    stmt = prepareSQLStatement(db, sql, stmt, SQLITE_OK,
                        "Failed to prepare update car statement: ", false);

    int driver_id = update.getDriverId();
    std::string license = update.getLicense();
    std::string brand = update.getBrand();
    double mileage = update.getMileageBuy();
    double load_capacity = update.getLoadCapacity();

    sqlite3_bind_int(stmt, 1, driver_id);
    sqlite3_bind_text(stmt, 2, license.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, brand.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 4, mileage);
    sqlite3_bind_double(stmt, 5, load_capacity);
    sqlite3_bind_int(stmt, 6, car_id);

    executeSQLStatement(db, stmt, SQLITE_DONE,
                        "Failed to execute update car statement: ", false);
}

void Controller::updateDriver(int user_id, Driver &update) {
    if (user.getRole() != ADMIN && user.getRole() != DISPATCHER) {
        throw PermissionDeniedException();
    }

    if (user.getRole() == DISPATCHER) {
        Dispatcher disp;
        disp.getDataFromDb(db, user.getId());
        Driver old;
        try {
            old.getDataFromDb(db, user_id);
        } catch (const std::exception &e) {
            throw std::invalid_argument("No driver found by provided id");
        }
        if (User::toLower(old.getCity()) != User::toLower(disp.getCity())) {
            throw PermissionDeniedException();
        }
    }

    Validator::validateUpdateDriver(update, user_id, db);

    char *sql = "UPDATE autopark_driver SET "
                "name = ?, surname = ?, "
                "category = ?, experience = ?, "
                "address = ?, city = ?, birthday = ? "
                "WHERE user_id = ?;";

    sqlite3_stmt *stmt;
    stmt = prepareSQLStatement(db, sql, stmt, SQLITE_OK,
                        "Failed to prepare update driver statement: ", false);

    std::string name = update.getName();
    std::string surname = update.getSurname();
    std::string category = update.getCategoryString();
    int experience = update.getExperience();
    std::string address = update.getAddress();
    std::string city = update.getCity();
    std::string birthday = update.getBirthday();

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, surname.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, category.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, experience);
    sqlite3_bind_text(stmt, 5, address.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, city.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 7, birthday.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 8, user_id);

    executeSQLStatement(db, stmt, SQLITE_DONE,
                        "Failed to execute update driver statement: ", false);
}

void Controller::updateDispatcher(int user_id, Dispatcher &update) {
    if (user.getRole() != ADMIN) {
        throw PermissionDeniedException();
    }

    Validator::validateUpdateDispatcher(update, user_id, db);

    char *sql = "UPDATE autopark_dispatcher SET "
                "name = ?, surname = ?, "
                "address = ?, city = ? "
                "WHERE user_id = ?;";

    sqlite3_stmt *stmt = nullptr;
    stmt = prepareSQLStatement(db, sql, stmt, SQLITE_OK,
                        "Failed to prepare update dispatcher statement: ", false);

    std::string name = update.getName();
    std::string surname = update.getSurname();
    std::string address = update.getAddress();
    std::string city = update.getCity();

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, surname.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, address.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, city.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 5, user_id);

    executeSQLStatement(db, stmt, SQLITE_DONE,
                        "Failed to execute update dispatcher statement: ", false);
}

void Controller::updateOrder(int order_id, Order &update) {
    if (user.getRole() != ADMIN && user.getRole() != DISPATCHER) {
        throw PermissionDeniedException();
    }

    Validator::validateUpdateOrder(update, order_id, db);

    char *sql = "UPDATE autopark_order SET "
                "driver_id = ?, car_id = ?, "
                "date = ?, mileage = ?, "
                "load = ?, cost = ?,"
                "is_approved = ? "
                "WHERE id = ?;";

    sqlite3_stmt *stmt;
    stmt = prepareSQLStatement(db, sql, stmt, SQLITE_OK,
                               "Failed to prepare update dispatcher statement: ", false);

    int driver_id = update.getDriverId();
    int car_id = update.getCarId();
    std::string date = update.getDate();
    double mileage = update.getMileage();
    double load = update.getCost();
    double cost = update.getLoad();
    int is_approved = update.getIsApproved();

    sqlite3_bind_int(stmt, 1, driver_id);
    sqlite3_bind_int(stmt, 2, car_id);
    sqlite3_bind_text(stmt, 3, date.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 4, mileage);
    sqlite3_bind_double(stmt, 5, load);
    sqlite3_bind_double(stmt, 6, cost);
    sqlite3_bind_int(stmt, 7, is_approved);
    sqlite3_bind_int(stmt, 8, order_id);

    executeSQLStatement(db, stmt, SQLITE_DONE,
                        "Failed to execute update dispatcher statement: ", false);
}

void Controller::updateUser(int user_id, User &update) {
    if (user.getRole() != ADMIN && user_id != user.getId()) {
        throw PermissionDeniedException();
    }

    Validator::validateUpdateUser(update, user_id, db);

    char* sql = "UPDATE autopark_user SET "
                "login = ?, pass_hash = ? "
                "WHERE id = ?;";
    sqlite3_stmt *stmt;
    stmt = prepareSQLStatement(db, sql, stmt, SQLITE_OK,
                        "Failed to prepare update user statement", false);

    std::string login = update.getLogin();
    std::string pass_hash = update.getPassHash();

    sqlite3_bind_text(stmt, 1, login.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, pass_hash.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, user_id);

    executeSQLStatement(db, stmt, SQLITE_DONE,
                        "Failed to execute update user statement", false);
}