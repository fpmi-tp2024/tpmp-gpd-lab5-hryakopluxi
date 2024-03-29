//
// Created by Stanislau Senkevich on 21.3.24.
//

#ifndef LAB_5_PROJECT_VIEW_H
#define LAB_5_PROJECT_VIEW_H

#include "controller.h"
#include <iostream>

class View {
private:
    std::string db_filename = "db/app.db";
    Controller controller;

    static const std::string GUEST_MENU_MSG;
    static const std::string DRIVER_MENU_MSG;
    static const std::string DISPATCHER_MENU_MSG;
    static const std::string ADMIN_MENU_MSG;

    void login();
    void logout();
    void changeDb();

    bool guestMenu();
    bool driverMenu();
    bool dispatcherMenu();
    bool adminMenu();

    void getDriverOrders(int driver_id) const;
    void getCarSummaryMileageAndLoads() const;
    void getDriverStatistics(int driver_id) const;
    void getAllDriverStatistics() const;
    void getWorstDriverStatistics() const;
    void getInfoAboutCarWithMaxMileage() const;

    void storeDriversEarnedMoney();
    void getDriverEarnedMoney(int driver_id) const;

    void addCar();
    void addDriver();
    void addOrder();
    void addDispatcher();

    void updateCar();
    void updateDriver();
    void updateOrder();
    void updateOrderApproveStatus();
    void updateDispatcher();
    void updateUser(int user_id);

    void deleteCar();
    void deleteDriver();
    void deleteOrder();
    void deleteDispatcher();

    void pressAnyKey() const;

public:
    View();
    void menu();
};

#endif //LAB_5_PROJECT_VIEW_H
