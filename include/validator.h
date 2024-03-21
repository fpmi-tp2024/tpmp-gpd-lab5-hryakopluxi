//
// Created by hakeyn on 21.3.24.
//

#ifndef LAB_5_PROJECT_VALIDATOR_H
#define LAB_5_PROJECT_VALIDATOR_H

#include "config.h"
#include "order.h"
#include "driver.h"

class Validator {
public:
    static bool validOrder(const Order& order);
    static bool validDriver(const Driver& driver);
    static bool validLicense(std::string license);
};

#endif //LAB_5_PROJECT_VALIDATOR_H
