//
// Created by Stanislau Senkevich on 21.3.24.
//

#ifndef LAB_5_PROJECT_CONFIG_H
#define LAB_5_PROJECT_CONFIG_H

#include <string>
#include <sqlite3.h>
#include "sql.h"

class Config {
private:
    double driverPercent = 20.;

public:
    Config() = default;
    Config(sqlite3* db, const std::string& config_name);
    [[nodiscard]] double getDriverMul() const {
        return driverPercent / 100.;
    }

    void setDriverPercent(double newDriverPercent) {
        driverPercent = newDriverPercent;
    }
};



#endif //LAB_5_PROJECT_CONFIG_H
