//
// Created by hakeyn on 21.3.24.
//

#ifndef LAB_5_PROJECT_CAR_H
#define LAB_5_PROJECT_CAR_H

#include <string>
#include <utility>
#include <sqlite3.h>
#include "exceptions.h"
#include "sql.h"

class Car {

private:
    int id;
    int driver_id;
    std::string license;
    std::string brand;
    double mileage_buy;
    double load_capacity;
public:
    Car() : id(0), driver_id(0), license(""), brand(""), mileage_buy(0.0), load_capacity(0.0) {}

    Car(int newId, int newDriverId, std::string newLicense, std::string newBrand, double newMileageBuy,
        double newLoadCapacity)
            : id(newId), driver_id(newDriverId), license(std::move(newLicense)), brand(std::move(newBrand)),
              mileage_buy(newMileageBuy), load_capacity(newLoadCapacity) {}

    int getId() const {
        return id;
    }

    int getDriverId() const {
        return driver_id;
    }

    std::string getLicense() const {
        return license;
    }

    std::string getBrand() const {
        return brand;
    }

    double getMileageBuy() const {
        return mileage_buy;
    }

    double getLoadCapacity() const {
        return load_capacity;
    }

    void setId(int newId) {
        id = newId;
    }

    void setDriverId(int newDriverId) {
        driver_id = newDriverId;
    }

    void setLicense(const std::string &newLicense) {
        license = newLicense;
    }

    void setBrand(const std::string &newBrand) {
        brand = newBrand;
    }

    void setMileageBuy(double newMileageBuy) {
        mileage_buy = newMileageBuy;
    }

    void setLoadCapacity(double newLoadCapacity) {
        load_capacity = newLoadCapacity;
    }

    void getDataFromDb(sqlite3 *db, int car_id);
    void insertCarToDb(sqlite3 *db);
};

#endif //LAB_5_PROJECT_CAR_H
