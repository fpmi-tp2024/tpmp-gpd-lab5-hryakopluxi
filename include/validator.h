//
// Created by hakeyn on 21.3.24.
//

#ifndef LAB_5_PROJECT_VALIDATOR_H
#define LAB_5_PROJECT_VALIDATOR_H

#include "config.h"
#include "order.h"
#include "driver.h"

class Validator {
    const Config* config;

public:
    bool validOrder(const Order& order) const;
    bool validDriver(const Driver& driver) const;
    bool validLicense(std::string license) const;
    bool validPassword(std::string password, std::string salt) const;
};

#endif //LAB_5_PROJECT_VALIDATOR_H
