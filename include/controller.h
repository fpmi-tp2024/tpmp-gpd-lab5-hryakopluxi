//
// Created by hakeyn on 21.3.24.
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
    sqlite3* db;
    User user;
    Config config;
public:

    Controller(const std::string& db_filename);

    bool login(const std::string& login, const std::string& password);
    void logout();

    Driver getDriverInfo(int driver_id) const;
    Order getOrderInfo(int order_id) const;
    Car getCarInfo(int car_id) const;
    std::vector <Order>& getDriverOrders(int driver_id, std::string period) const;
    int getWorstDriverId (std::string period) const;
    int getHighestMileageCarId() const;

    void addCar(Car& car);
    void addDriver(Driver& driver);
    void addOrder(Order& order);
    void addDispatcher(Dispatcher& dispatcher);

    void updateCar(int car_id, Car& update);
    void updateDriver(int user_id, Driver& update);
    void updateDispatcher(int user_id, Dispatcher& update);
    void updateOrder(int order_id, Order& update);
    void updateUser(int user_id, User& update);
    void approveOrder(int order_id);

    void deleteCar(int car_id);
    void deleteDriver(int user_id);
    void deleteOrder(int order_id);
    void deleteDispatcher(int user_id);

    void storeDriversEarnedMoney(std::string period);
    void getDriverEarnedMoney(int driver_id);
};

#endif //LAB_5_PROJECT_CONTROLLER_H
