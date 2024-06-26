//
// Created by Stanislau Senkevich on 21.3.24.
//

#include "../include/controller.h"

Controller::Controller(const std::string &db_filename) {
    db = nullptr;
    int rc = sqlite3_open_v2(db_filename.c_str(), &db, SQLITE_OPEN_READWRITE, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close_v2(db);
        throw std::runtime_error("Cannot open database");
    }
    user = User();
    config = Config(db, "dev");
}

bool Controller::login(const std::string &login, const std::string &password) {
    std::string sql = "SELECT * FROM autopark_user WHERE login = ? ;";

    sqlite3_stmt *stmt = SQL::prepareSQLStatement(db, sql, "Failed to prepare login statement: ");

    sqlite3_bind_text(stmt, 1, login.c_str(), -1, SQLITE_STATIC);

    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        return false;
    }

    std::string pass_hash = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
    if (!Validator::validatePassword(pass_hash, password)) {
        return false;
    }

    user.setId(sqlite3_column_int(stmt, 0));
    user.setLogin(login);
    user.setRole(static_cast<Role>(sqlite3_column_int(stmt, 3)));
    return true;
}

void Controller::logout() {
    user = User();
}


int Controller::addCar(Car &car) {
    if (user.getRole() != ADMIN && user.getRole() != DISPATCHER) {
        throw PermissionDeniedException();
    }

    Validator::validCar(car, db);

    return car.insertCarToDb(db);
}

int Controller::addDriver(Driver &driver) {
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
    driver.setPassHash(Validator::hashPassword(driver.getPassHash()));
    driver.setRole(DRIVER);

    return driver.insertUserToDb(db);
}

int Controller::addOrder(Order &order) {
    if (user.getRole() != ADMIN && user.getRole() != DISPATCHER && order.getDriverId() != user.getId()) {
        throw PermissionDeniedException();
    }

    if (user.getRole() == DISPATCHER) {
        Driver driver;
        Dispatcher dispatcher;
        dispatcher.getDataFromDb(db, user.getId());
        driver.getDataFromDb(db, order.getDriverId());
        if (User::toLower(dispatcher.getCity()) != User::toLower(driver.getCity())) {
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

    return order.insertOrderToDb(db);
}

int Controller::addDispatcher(Dispatcher &dispatcher) {
    if (user.getRole() != ADMIN) {
        throw PermissionDeniedException();
    }

    Validator::validDispatcher(dispatcher);
    dispatcher.setPassHash(Validator::hashPassword(dispatcher.getPassHash()));
    dispatcher.setRole(DISPATCHER);

    return dispatcher.insertUserToDb(db);
}

void Controller::deleteCar(int car_id) {
    if (user.getRole() != ADMIN) {
        throw PermissionDeniedException();
    }

    try {
        Car c;
        c.getDataFromDb(db, car_id);
    } catch (const std::exception &e) {
        throw std::invalid_argument("No car with provided id");
    }

    std::string sql = "DELETE FROM autopark_car WHERE id = ?";
    sqlite3_stmt *stmt = SQL::prepareSQLStatement(db, sql,
                                    "Failed to prepare delete car statement: ");


    sqlite3_bind_int(stmt, 1, car_id);
    SQL::executeSQLStatement(db, stmt, SQLITE_DONE,
                             "Failed to execute delete car statement: ");
}

void Controller::deleteDriver(int user_id) {
    if (user.getRole() != ADMIN) {
        throw PermissionDeniedException();
    }

    try {
        Driver d;
        d.getDataFromDb(db, user_id);
    } catch (const std::exception &e) {
        throw std::invalid_argument("No driver with provided id");
    }

    sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);

    std::string sql = "DELETE FROM autopark_driver WHERE user_id = ?";

    sqlite3_stmt *stmt = SQL::prepareSQLStatement(db, sql,
                                    "Failed to prepare delete driver statement: ", true);


    sqlite3_bind_int(stmt, 1, user_id);
    SQL::executeSQLStatement(db, stmt, SQLITE_DONE,
                             "Failed to execute delete driver statement: ", true);

    sql = "DELETE FROM autopark_user WHERE id = ?";
    stmt = SQL::prepareSQLStatement(db, sql,
                                    "Failed to prepare delete user statement: ", true);


    sqlite3_bind_int(stmt, 1, user_id);
    SQL::executeSQLStatement(db, stmt, SQLITE_DONE,
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
        throw std::invalid_argument("No dispatcher with provided id");
    }

    sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);

    std::string sql = "DELETE FROM autopark_dispatcher WHERE user_id = ?";

    sqlite3_stmt *stmt = SQL::prepareSQLStatement(db, sql,
                                    "Failed to prepare delete dispatcher statement: ", true);

    sqlite3_bind_int(stmt, 1, user_id);
    SQL::executeSQLStatement(db, stmt, SQLITE_DONE,
                             "Failed to execute delete dispatcher statement: ", true);

    sql = "DELETE FROM autopark_user WHERE id = ?";
    stmt = SQL::prepareSQLStatement(db, sql,
                                    "Failed to prepare delete user statement: ", true);

    sqlite3_bind_int(stmt, 1, user_id);
    SQL::executeSQLStatement(db, stmt, SQLITE_DONE,
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
        throw std::invalid_argument("No order with provided id");
    }

    std::string sql = "DELETE FROM autopark_order WHERE id = ?";

    sqlite3_stmt *stmt = SQL::prepareSQLStatement(db, sql,
                                    "Failed to prepare delete order statement: ");

    sqlite3_bind_int(stmt, 1, order_id);
    SQL::executeSQLStatement(db, stmt, SQLITE_DONE,
                             "Failed to execute delete order statement: ");
}

void Controller::updateCar(int car_id, Car &update) {
    if (user.getRole() != ADMIN && user.getRole() != DISPATCHER) {
        throw PermissionDeniedException();
    }

    Validator::validateUpdateCar(update, car_id, db);

    std::string sql = "UPDATE autopark_car SET "
                      "driver_id = ?, license = ?, brand = ?, "
                      "mileage = ?, load_capacity = ? "
                      "WHERE id = ?;";


    sqlite3_stmt *stmt = SQL::prepareSQLStatement(db, sql,
                                    "Failed to prepare update car statement: ");

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

    SQL::executeSQLStatement(db, stmt, SQLITE_DONE,
                             "Failed to execute update car statement: ");
}

void Controller::updateDriver(int user_id, Driver &update) {
    if (user.getRole() != ADMIN && user.getRole() != DISPATCHER) {
        throw PermissionDeniedException();
    }

    if (user.getRole() == DISPATCHER) {
        Dispatcher dispatcher;
        dispatcher.getDataFromDb(db, user.getId());
        Driver old;
        try {
            old.getDataFromDb(db, user_id);
        } catch (const std::exception &e) {
            throw std::invalid_argument("No driver found by provided id");
        }
        if (User::toLower(old.getCity()) != User::toLower(dispatcher.getCity())) {
            throw PermissionDeniedException();
        }
    }

    Validator::validateUpdateDriver(update, user_id, db);

    std::string sql = "UPDATE autopark_driver SET "
                      "name = ?, surname = ?, "
                      "category = ?, experience = ?, "
                      "address = ?, city = ?, birthday = ? "
                      "WHERE user_id = ?;";


    sqlite3_stmt *stmt = SQL::prepareSQLStatement(db, sql,
                                    "Failed to prepare update driver statement: ");

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

    SQL::executeSQLStatement(db, stmt, SQLITE_DONE,
                             "Failed to execute update driver statement: ");
}

void Controller::updateDispatcher(int user_id, Dispatcher &update) {
    if (user.getRole() != ADMIN) {
        throw PermissionDeniedException();
    }

    Validator::validateUpdateDispatcher(update, user_id, db);

    std::string sql = "UPDATE autopark_dispatcher SET "
                      "name = ?, surname = ?, "
                      "address = ?, city = ? "
                      "WHERE user_id = ?;";


    sqlite3_stmt *stmt = SQL::prepareSQLStatement(db, sql,
                                    "Failed to prepare update dispatcher statement: ");

    std::string name = update.getName();
    std::string surname = update.getSurname();
    std::string address = update.getAddress();
    std::string city = update.getCity();

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, surname.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, address.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, city.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 5, user_id);

    SQL::executeSQLStatement(db, stmt, SQLITE_DONE,
                             "Failed to execute update dispatcher statement: ");
}

void Controller::updateOrder(int order_id, Order &update) {
    if (user.getRole() != ADMIN && user.getRole() != DISPATCHER) {
        throw PermissionDeniedException();
    }

    Validator::validateUpdateOrder(update, order_id, db);

    std::string sql = "UPDATE autopark_order SET "
                      "driver_id = ?, car_id = ?, "
                      "date = ?, mileage = ?, "
                      "load = ?, cost = ?,"
                      "is_approved = ? "
                      "WHERE id = ?;";


    sqlite3_stmt *stmt = SQL::prepareSQLStatement(db, sql,
                                    "Failed to prepare update dispatcher statement: ");

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

    SQL::executeSQLStatement(db, stmt, SQLITE_DONE,
                             "Failed to execute update dispatcher statement: ");
}

void Controller::updateUser(int user_id, User &update) {
    if (user.getRole() != ADMIN && user_id != user.getId()) {
        throw PermissionDeniedException();
    }

    Validator::validateUpdateUser(update, user_id, db);

    std::string sql = "UPDATE autopark_user SET "
                      "login = ?, pass_hash = ? "
                      "WHERE id = ?;";

    sqlite3_stmt *stmt = SQL::prepareSQLStatement(db, sql,
                                    "Failed to prepare update user statement: ");

    std::string login = update.getLogin();
    std::string pass_hash = update.getPassHash();

    sqlite3_bind_text(stmt, 1, login.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, pass_hash.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, user_id);

    SQL::executeSQLStatement(db, stmt, SQLITE_DONE,
                             "Failed to execute update user statement: ");
}

void Controller::updateOrderApproveStatus(int order_id, bool status) {
    Order ord;

    try {
        ord.getDataFromDb(db, order_id);
    } catch (const std::exception &e) {
        throw std::invalid_argument("No order found with provided id: ");
    }

    if (ord.getIsApproved() == status) {
        if (status) {
            throw std::invalid_argument("Order already approved");
        }
        throw std::invalid_argument("Order already not approved");
    }


    std::string sql = "UPDATE autopark_order SET is_approved = ? WHERE id = ?;";
    sqlite3_stmt *stmt = SQL::prepareSQLStatement(db, sql,
                                    "Failed to prepare update approve status order statement: ");

    sqlite3_bind_int(stmt, 1, status);
    sqlite3_bind_int(stmt, 2, order_id);
    SQL::executeSQLStatement(db, stmt, SQLITE_DONE,
                             "Failed to execute update approve status order statement");
}

std::vector<Order> Controller::getDriverOrders(int driver_id, const std::string &date_start,
                                               const std::string &date_end) const {
    Driver driver;
    try {
        driver.getDataFromDb(db, driver_id);
    } catch (const std::exception &e) {
        throw std::invalid_argument("No driver was found");
    }

    if (user.getRole() == DISPATCHER) {
        Dispatcher dispatcher;
        dispatcher.getDataFromDb(db, user.getId());
        if (User::toLower(dispatcher.getCity()) != User::toLower(driver.getCity())) {
            throw PermissionDeniedException();
        }
    }

    if (user.getRole() == DRIVER && user.getId() != driver_id) {
        throw PermissionDeniedException();
    }

    if(!Validator::validPeriod(date_start, date_end)) {
        throw std::invalid_argument("Invalid period provided");
    }

    std::string sql = "SELECT id\n"
                      "FROM autopark_order\n"
                      "WHERE driver_id = ? \n"
                      "AND date >= DATE(?) AND date <= DATE(?);";
    sqlite3_stmt *stmt = SQL::prepareSQLStatement(db, sql,
                                    "Failed to prepare retrieve orders statement: ");
    sqlite3_bind_int(stmt, 1, driver_id);
    sqlite3_bind_text(stmt, 2, date_start.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, date_end.c_str(), -1, SQLITE_STATIC);

    int rc;
    std::vector<Order> orders;
    while (true) {
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_ROW) {
            break;
        }
        Order ord;
        ord.getDataFromDb(db, sqlite3_column_int(stmt, 0));
        orders.push_back(ord);
    }

    return orders;
}

std::string Controller::getCarSummaryMileageAndLoad(int car_id) const {
    Car car;
    try {
        car.getDataFromDb(db, car_id);
    } catch (const std::exception &e) {
        if (user.getRole() == DRIVER) {
            throw PermissionDeniedException();
        }
        throw std::invalid_argument("No car was found with provided id");
    }

    if (user.getRole() != ADMIN && user.getRole() != DISPATCHER && user.getId() != car.getDriverId()) {
        throw PermissionDeniedException();
    }

    std::string sql = "SELECT SUM(load), SUM(mileage) "
                      "FROM autopark_order "
                      "WHERE car_id = ?;";

    sqlite3_stmt *stmt = SQL::prepareSQLStatement(db, sql,
                                    "Failed to prepare car info statement: ");
    sqlite3_bind_int(stmt, 1, car_id);

    stmt = SQL::executeSQLStatement(db, stmt, SQLITE_ROW,
                                    "Failed tp execute car info statement: ",
                                    false, false);
    double load = sqlite3_column_double(stmt, 0);
    double mileage = sqlite3_column_double(stmt, 1);
    sqlite3_finalize(stmt);

    char buf[100];
    sprintf(buf, "Summary load: %.3f\nSummary mileage: %.1f\n", load, mileage + car.getMileageBuy());
    return buf;
}

std::string Controller::getDriverStat(int driver_id) const {
    std::string sql = "SELECT u.id, u.login, d.name, "
                      "d.surname, d.category, d.experience, "
                      "d.city, d.address, d.birthday, "
                      "COUNT(o.driver_id), SUM(cost) * ?, SUM(mileage) "
                      "FROM autopark_driver d "
                      "INNER JOIN autopark_user u ON u.id = d.user_id "
                      "LEFT JOIN autopark_order o ON o.driver_id = d.user_id "
                      "WHERE u.id = ?;";


    sqlite3_stmt *stmt = SQL::prepareSQLStatement(db, sql,
                                    "Failed to prepare driver summary statement: ");
    sqlite3_bind_double(stmt, 1, config.getDriverMul());
    sqlite3_bind_int(stmt, 2, driver_id);
    SQL::executeSQLStatement(db, stmt, SQLITE_ROW,
                             "Failed to execute driver summary statement: ",
                             false, false);

    char response[1000];
    sprintf(response, "ID: %d\n"
                      "Login: %s\n"
                      "Name: %s\n"
                      "Surname: %s\n"
                      "Category: %s\n"
                      "Experience: %d years\n"
                      "City: %s\n"
                      "Address: %s\n"
                      "Birthday: %s\n"
                      "Orders completed: %d\n"
                      "Money earned: %.2f\n"
                      "Summary mileage: %.2f\n",
            sqlite3_column_int(stmt, 0), sqlite3_column_text(stmt, 1), sqlite3_column_text(stmt, 2),
            sqlite3_column_text(stmt, 3), sqlite3_column_text(stmt, 4), sqlite3_column_int(stmt, 5),
            sqlite3_column_text(stmt, 6), sqlite3_column_text(stmt, 7), sqlite3_column_text(stmt, 8),
            sqlite3_column_int(stmt, 9), sqlite3_column_double(stmt, 10), sqlite3_column_double(stmt, 11));
    return response;
}

std::string Controller::getDriverStatistics(int driver_id) const {
    if (user.getRole() != ADMIN && user.getRole() != DISPATCHER && user.getId() != driver_id) {
        throw PermissionDeniedException();
    }
    Driver driver;
    try {
        driver.getDataFromDb(db, driver_id);
    } catch (const std::exception &e) {
        throw std::invalid_argument("No driver found with provided id");
    }

    if (user.getRole() == DISPATCHER) {
        Dispatcher dispatcher;
        dispatcher.getDataFromDb(db, user.getId());
        if (User::toLower(dispatcher.getCity()) != User::toLower(driver.getCity())) {
            throw PermissionDeniedException();
        }
    }

    return getDriverStat(driver_id);
}

std::vector<std::string> Controller::getAllDriversStatistics() const {
    if (user.getRole() != ADMIN && user.getRole() != DISPATCHER) {
        throw PermissionDeniedException();
    }

    std::string city;

    std::string sql = "SELECT u.id, u.login, d.name, "
                      "d.surname, d.category, d.experience, "
                      "d.city, d.address, d.birthday, "
                      "COUNT(o.driver_id), SUM(cost), SUM(mileage) "
                      "FROM autopark_driver d "
                      "INNER JOIN autopark_user u ON u.id = d.user_id "
                      "LEFT JOIN autopark_order o ON o.driver_id = d.user_id ";

    if (user.getRole() == DISPATCHER) {
        Dispatcher dispatcher;
        dispatcher.getDataFromDb(db, user.getId());
        sql += "WHERE d.city = ? ";
        city = User::toLower(dispatcher.getCity());
    }

    sql += "GROUP BY d.user_id;";


    sqlite3_stmt *stmt = SQL::prepareSQLStatement(db, sql,
                                    "Failed to prepare all drivers statistics statement: ");

    if (user.getRole() == DISPATCHER) {
        sqlite3_bind_text(stmt, 1, city.c_str(), -1, SQLITE_STATIC);
    }

    std::vector<std::string> data;
    while (true) {
        int rc = sqlite3_step(stmt);
        if (rc != SQLITE_ROW) {
            break;
        }
        char response[1000];
        sprintf(response, "ID: %d\n"
                          "Login: %s\n"
                          "Name: %s\n"
                          "Surname: %s\n"
                          "Category: %s\n"
                          "Experience: %d years\n"
                          "City: %s\n"
                          "Address: %s\n"
                          "Birthday: %s\n"
                          "Orders completed: %d\n"
                          "Money earned: %.2f\n"
                          "Summary mileage: %.2f\n",
                sqlite3_column_int(stmt, 0), sqlite3_column_text(stmt, 1), sqlite3_column_text(stmt, 2),
                sqlite3_column_text(stmt, 3), sqlite3_column_text(stmt, 4), sqlite3_column_int(stmt, 5),
                sqlite3_column_text(stmt, 6), sqlite3_column_text(stmt, 7), sqlite3_column_text(stmt, 8),
                sqlite3_column_int(stmt, 9), sqlite3_column_double(stmt, 10), sqlite3_column_double(stmt, 11));
        data.push_back(response);
    }

    if (data.empty()) {
        data.push_back("No statistics were found.\n");
    }

    return data;
}

std::string Controller::getWorstDriverStatistics() const {
    if (user.getRole() != ADMIN && user.getRole() != DISPATCHER) {
        throw PermissionDeniedException();
    }

    std::string sql = "SELECT d.user_id, COALESCE(COUNT(o.driver_id), 0) AS num_approved_orders "
                      "FROM autopark_driver AS d "
                      "LEFT JOIN ("
                      "SELECT driver_id, COUNT(*) AS num_approved_orders "
                      "FROM autopark_order "
                      "WHERE is_approved = 1 "
                      "GROUP BY driver_id "
                      ") AS o ON d.user_id = o.driver_id "
                      "GROUP BY d.user_id "
                      "ORDER BY num_approved_orders ASC "
                      "LIMIT 1;";


    sqlite3_stmt *stmt = SQL::prepareSQLStatement(db, sql,
                                    "Failed to prepare search of worst driver's id: ");
    SQL::executeSQLStatement(db, stmt, SQLITE_ROW,
                             "Failed to execute search of worst driver's id: ",
                             false, false);
    int id = sqlite3_column_int(stmt, 0);

    return getDriverStat(id);
}

std::string Controller::getInfoAboutCarWithMaxMileage() const {
    if (user.getRole() != ADMIN && user.getRole() != DISPATCHER) {
        throw PermissionDeniedException();
    }

    std::string sql = "SELECT id "
                      "FROM ( "
                      "SELECT id, "
                      "SUM(mileage) AS total_mileage "
                      "FROM ( "
                      "SELECT id, mileage "
                      "FROM autopark_car "
                      "UNION ALL "
                      "SELECT car_id, "
                      "SUM(mileage) AS mileage "
                      "FROM autopark_order "
                      "GROUP BY car_id "
                      ") AS combined_mileage "
                      "GROUP BY id "
                      "ORDER BY total_mileage DESC "
                      "LIMIT 1 "
                      ") AS car_with_max_mileage;";



    sqlite3_stmt *stmt = SQL::prepareSQLStatement(db, sql,
                                    "Failed to prepare retrieving car id with max mileage: ");
    SQL::executeSQLStatement(db, stmt, SQLITE_ROW,
                             "Failed to execute retrieving car id with max mileage: ",
                             false, false);

    int id = sqlite3_column_int(stmt, 0);

    sql = "SELECT c.id, d.name, d.surname, c.driver_id, c.license, "
          "c.brand, c.mileage, c.load_capacity, "
          "SUM(o.cost), SUM(o.load), SUM(o.mileage) "
          "FROM autopark_car AS c "
          "LEFT JOIN autopark_order AS o ON o.car_id = c.id "
          "LEFT JOIN autopark_driver AS d ON d.user_id = c.driver_id "
          "WHERE c.id = ? AND o.is_approved = true;";
    stmt = SQL::prepareSQLStatement(db, sql,
                                    "Failed to prepare retrieving info about car with max mileage: ");
    sqlite3_bind_int(stmt, 1, id);
    SQL::executeSQLStatement(db, stmt, SQLITE_ROW,
                             "Failed to execute retrieving car id with max mileage: ",
                             false, false);
    char response[1000];

    sprintf(response, "ID: %d\n"
                      "Driver info: %s %s, id: %d\n"
                      "License: %s\n"
                      "Brand: %s\n"
                      "Mileage on purchase: %.2f\n"
                      "Load capacity: %.2f\n"
                      "Summary cost of completed orders: %.2f\n"
                      "Summary load: %.2f\n"
                      "Summary mileage: %.2f\n",
            sqlite3_column_int(stmt, 0), sqlite3_column_text(stmt, 1), sqlite3_column_text(stmt, 2),
            sqlite3_column_int(stmt, 3), sqlite3_column_text(stmt, 4), sqlite3_column_text(stmt, 5),
            sqlite3_column_double(stmt, 6), sqlite3_column_double(stmt, 7), sqlite3_column_double(stmt, 8),
            sqlite3_column_double(stmt, 9), sqlite3_column_double(stmt, 10) + sqlite3_column_double(stmt, 6));

    return response;
}

std::vector<std::string> Controller::storeDriversEarnedMoney(
        const std::string& start_date, const std::string& end_date) {
    if (user.getRole() != ADMIN) {
        throw PermissionDeniedException();
    }

    if (!Validator::validPeriod(start_date, end_date)) {
        throw std::invalid_argument("Invalid period provided");
    }

    sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);

    std::string sql = "DELETE FROM autopark_earning_info "
                      "WHERE start_date = ? AND end_date = ?;";


    sqlite3_stmt *stmt = SQL::prepareSQLStatement(db, sql,
                             "Failed to prepare deleting old rows about earnings: ",
                             true);


    sqlite3_bind_text(stmt, 1, start_date.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, end_date.c_str(), -1, SQLITE_STATIC);


    SQL::executeSQLStatement(db, stmt, SQLITE_DONE,
                             "Failed to execute deleting old rows about earnings: ",true);

    sql = "INSERT INTO autopark_earning_info (driver_id, surname, money, start_date, end_date)"
          "SELECT driver_id, d.surname, "
          "COALESCE(SUM(amount) * ?, 0) AS money, "
          "? AS start_date, "
          "? AS end_date "
          "FROM ( "
          "SELECT user_id AS driver_id, '' AS surname, 0 AS amount "
          "FROM autopark_driver "
          "WHERE user_id NOT IN ( "
          "SELECT DISTINCT driver_id "
          "FROM autopark_order "
          "WHERE date BETWEEN ? AND ? AND is_approved = 1 "
          ") "
          "UNION ALL "
          "SELECT driver_id, '', SUM(cost) AS amount "
          "FROM autopark_order "
          "WHERE date BETWEEN ? AND ? "
          "GROUP BY driver_id "
          ") AS earnings "
          "LEFT JOIN autopark_driver AS d ON earnings.driver_id = d.user_id "
          "GROUP BY driver_id; ";

    stmt = SQL::prepareSQLStatement(db, sql,"Failed to prepare inserting new info: ",true);
    sqlite3_bind_double(stmt, 1, config.getDriverMul());
    sqlite3_bind_text(stmt, 2, start_date.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, end_date.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, start_date.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, end_date.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, start_date.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 7, end_date.c_str(), -1, SQLITE_STATIC);

    SQL::executeSQLStatement(db, stmt, SQLITE_DONE,
                             "Failed to execute inserting new info: ",
                             true);

    sql = "SELECT driver_id, surname, money FROM autopark_earning_info "
          "WHERE start_date = ? AND end_date = ?;";
    stmt = SQL::prepareSQLStatement(
            db, sql, "Failed to prepare selecting result statement", true);
    sqlite3_bind_text(stmt, 1, start_date.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, end_date.c_str(), -1, SQLITE_STATIC);

    std::vector<std::string> response;

    while (true) {
        int rc = sqlite3_step(stmt);
        if (rc != SQLITE_ROW) {
            break;
        }
        char buf[1000];
        sprintf(buf,"Driver ID: %d\nSurname: %s\nMoney earned: %.2f\n",
                     sqlite3_column_int(stmt, 0), sqlite3_column_text(stmt, 1),
                     sqlite3_column_double(stmt, 2));
        response.push_back(buf);
    }
    sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
    return response;
}

double Controller::getDriverEarnedMoney(
        int driver_id, const std::string& start_date, const std::string& end_date) const {
    if (user.getRole() != ADMIN && user.getRole() != DISPATCHER && driver_id != user.getId()) {
        throw PermissionDeniedException();
    }

    if (!Validator::validPeriod(start_date, end_date)) {
        throw std::invalid_argument("Invalid period provided");
    }

    Driver d;
    try {
        d.getDataFromDb(db, driver_id);
    } catch (const std::exception& e) {
        throw std::invalid_argument("No driver was found by provided id.");
    }

    if (user.getRole() == DISPATCHER) {
        Dispatcher dispatcher;
        dispatcher.getDataFromDb(db, user.getId());
        if (User::toLower(dispatcher.getCity()) != User::toLower(d.getCity())) {
            throw PermissionDeniedException();
        }
    }

    std::string sql = "SELECT SUM(cost) * ? "
                      "FROM autopark_order "
                      "WHERE date BETWEEN ? AND ? AND is_approved = 1;";


    sqlite3_stmt *stmt = SQL::prepareSQLStatement(db, sql,
                                    "Failed to prepare retrieving driver income statement: ");
    sqlite3_bind_double(stmt, 1, config.getDriverMul());
    sqlite3_bind_text(stmt, 2, start_date.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, end_date.c_str(), -1, SQLITE_STATIC);

    SQL::executeSQLStatement(db, stmt, SQLITE_ROW,
                             "Failed to execute retrieving driver income statement: ",
                             false, false);

    double money = sqlite3_column_double(stmt, 0);
    sqlite3_finalize(stmt);

    return money;
}
