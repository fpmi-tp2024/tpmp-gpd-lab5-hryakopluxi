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
public:
    View();

    void login();
    void logout();

    void menu() const;

    void getDriverOrders() const;
    void getCarSummaryMileageAndLoads() const;
    void getDriverSummaryWork() const;
    void getWorstDriverSummary() const;
    void getBestCarSummary() const;

    void storeDriversEarnedMoney();
    void getDriverEarnedMoney();

    void addCar();
    void addDriver();
    void addOrder();
    void addDispatcher();

    void updateCar();
    void updateDriver();
    void updateOrder();
    void updateDispatcher();

    void deleteCar();
    void deleteDriver();
    void deleteOrder();
    void deleteDispatcher();

};

#endif //LAB_5_PROJECT_VIEW_H
