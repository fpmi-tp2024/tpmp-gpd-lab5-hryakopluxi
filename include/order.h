//
// Created by hakeyn on 21.3.24.
//

#ifndef LAB_5_PROJECT_ORDER_H
#define LAB_5_PROJECT_ORDER_H

#include <string>
#include <sqlite3.h>
#include "sql.h"

class Order {
private:
    int id;
    int driver_id;
    int car_id;
    std::string date;
    double mileage;
    double load;
    double cost;
    bool is_approved;

public:
    Order() : id(0), driver_id(0), car_id(0), date(""), mileage(0.0), load(0.0), cost(0.0), is_approved(false) {}

    Order(int newId, int newDriverId, int newCarId, const std::string &newDate, double newMileage, double newLoad,
          double newCost, bool newIsApproved)
            : id(newId), driver_id(newDriverId), car_id(newCarId), date(newDate), mileage(newMileage), load(newLoad),
              cost(newCost), is_approved(newIsApproved) {}

    int getId() const {
        return id;
    }

    int getDriverId() const {
        return driver_id;
    }

    int getCarId() const {
        return car_id;
    }

    std::string getDate() const {
        return date;
    }

    double getMileage() const {
        return mileage;
    }

    double getLoad() const {
        return load;
    }

    double getCost() const {
        return cost;
    }

    bool getIsApproved() const {
        return is_approved;
    }

    void setId(int newId) {
        id = newId;
    }

    void setDriverId(int newDriverId) {
        driver_id = newDriverId;
    }

    void setCarId(int newCarId) {
        car_id = newCarId;
    }

    void setDate(const std::string &newDate) {
        date = newDate;
    }

    void setMileage(double newMileage) {
        mileage = newMileage;
    }

    void setLoad(double newLoad) {
        load = newLoad;
    }

    void setCost(double newCost) {
        cost = newCost;
    }

    void setIsApproved(bool newIsApproved) {
        is_approved = newIsApproved;
    }

    void getDataFromDb(sqlite3 *db, int order_id);

    void insertOrderToDb(sqlite3 *db);
};

#endif //LAB_5_PROJECT_ORDER_H
