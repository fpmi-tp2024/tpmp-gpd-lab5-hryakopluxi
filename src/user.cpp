//
// Created by hakeyn on 21.3.24.
//

#include "../include/user.h"

std::string User::toLower(std::string str) {
    for (int i = 0; i < str.size(); i++) {
        str[i] = std::tolower(str[i]);
    }
    return str;
}