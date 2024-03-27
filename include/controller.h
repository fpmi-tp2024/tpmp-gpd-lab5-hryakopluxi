//
// Created by Stanislau Senkevich on 21.3.24.
//

#include <sqlite3.h>
#include "dispatcher.h"
#include "config.h"
#include "validator.h"
#include "sql.h"
#include "bcrypt/BCrypt.hpp"

#ifndef LAB_5_PROJECT_CONTROLLER_H
#define LAB_5_PROJECT_CONTROLLER_H

class Controller {
private:
    sqlite3* db = nullptr;
    User user;
    Config config;

    std::string getDriverStat(int driver_id) const;
public:
    Controller() = default;
    explicit Controller(const std::string& db_filename);

    bool login(const std::string& login, const std::string& password);
    void logout();

    [[nodiscard]] std::vector <Order> getDriverOrders(int driver_id, const std::string& date_start, const std::string& date_end) const;
    [[nodiscard]] std::string getCarSummaryMileageAndLoad(int car_id) const;
    [[nodiscard]] std::string getDriverStatistics(int driver_id) const;
    [[nodiscard]] std::vector<std::string> getAllDriversStatistics() const;
    [[nodiscard]] std::string getWorstDriverSummary() const;
    [[nodiscard]] std::string getBestCarInfo() const;
    void storeDriversEarnedMoney(std::string period);
    void getDriverEarnedMoney(int driver_id);

    void addCar(Car& car);
    void addDriver(Driver& driver);
    void addOrder(Order& order);
    void addDispatcher(Dispatcher& dispatcher);

    void updateCar(int car_id, Car& update);
    void updateDriver(int user_id, Driver& update);
    void updateDispatcher(int user_id, Dispatcher& update);
    void updateOrder(int order_id, Order& update);
    void updateUser(int user_id, User& update);
    void updateOrderApproveStatus(int order_id, bool status);

    void deleteCar(int car_id);
    void deleteDriver(int user_id);
    void deleteOrder(int order_id);
    void deleteDispatcher(int user_id);

};

#endif //LAB_5_PROJECT_CONTROLLER_H
