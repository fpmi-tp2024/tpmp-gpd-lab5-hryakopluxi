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

    [[nodiscard]] std::string getDriverStat(int driver_id) const;
public:
    Controller() = default;
    explicit Controller(const std::string& db_filename);

    bool login(const std::string& login, const std::string& password);
    void logout();

    [[nodiscard]] int getUserId() const {return user.getId();}
    [[nodiscard]] std::vector <Order> getDriverOrders(int driver_id, const std::string& date_start, const std::string& date_end) const;
    [[nodiscard]] std::string getCarSummaryMileageAndLoad(int car_id) const;
    [[nodiscard]] std::string getDriverStatistics(int driver_id) const;
    [[nodiscard]] std::vector<std::string> getAllDriversStatistics() const;
    [[nodiscard]] std::string getWorstDriverStatistics() const;
    [[nodiscard]] std::string getInfoAboutCarWithMaxMileage() const;
    std::vector<std::string> storeDriversEarnedMoney(const std::string& start_date, const std::string& end_date);
    [[nodiscard]] double getDriverEarnedMoney(int driver_id, const std::string& start_date, const std::string& end_date) const;

    int addCar(Car& car);
    int addDriver(Driver& driver);
    int addOrder(Order& order);
    int addDispatcher(Dispatcher& dispatcher);

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
