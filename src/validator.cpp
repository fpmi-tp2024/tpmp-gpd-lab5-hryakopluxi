//
// Created by hakeyn on 21.3.24.
//

#include "../include/validator.h"
#include <regex>


bool Validator::validLicense(std::string license) {
    std::regex pattern("^[0-9]{4}[ABEIKMHOPCTX]{2}-[1-7]$");
    return std::regex_match(license, pattern);
}