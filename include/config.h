//
// Created by hakeyn on 21.3.24.
//

#ifndef LAB_5_PROJECT_CONFIG_H
#define LAB_5_PROJECT_CONFIG_H

#include <string>

class Config {
private:
    double driverPercent;
    std::string hashSalt;

public:
    double getDriverPercent() const {
        return driverPercent;
    }

    std::string getHashSalt() const {
        return hashSalt;
    }

    void setDriverPercent(double newDriverPercent) {
        driverPercent = newDriverPercent;
    }

    void setHashSalt(const std::string& newHashSalt) {
        hashSalt = newHashSalt;
    }
};



#endif //LAB_5_PROJECT_CONFIG_H
