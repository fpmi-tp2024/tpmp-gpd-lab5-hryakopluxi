//
// Created by Stanislau Senkevich on 30.3.24.
//

#include <gtest/gtest.h>
#include "../include/validator.h"

TEST(Validator_validName, TestPositive) {
    std::string name = "Stanislau";
    EXPECT_TRUE(Validator::validName(name) );
}

TEST(Validator_validName, TestNegative) {
    std::vector<std::pair<std::string, std::string>> table {
            {"", "Empty input"},
            {"Stas228", "Digits"},
            { "Stas-", "- sign"},
            {"Stas!", "! sign"},
    };

    for (const auto& s : table) {
        EXPECT_FALSE(Validator::validName(s.first))
        << "Test case: " << s.first << "\tTest name: " << s.second;
    }
}

TEST(Validator_validSurname, TestPositive) {}

TEST(Validator_validSurname, TestNegative) {}

TEST(Validator_validAddress, TestPositive) {}

TEST(Validator_validAddress, TestNegative) {}

TEST(Validator_validCity, TestPositive) {}

TEST(Validator_validCity, TestNegative) {}

TEST(Validator_validDate, TestPositive) {}

TEST(Validator_validDate, TestNegative) {
    std::vector<std::pair<std::string, std::string>> table {
            {"", "Empty input"},
            {"12-12-2004", "Invalid pattern"},
            { "1999-12-12", "Invalid year"},
            { "2024-13-24", "Invalid month"},
            { "2024-02-30", "Invalid month day"},
            { "2023-02-29", "Invalid month day"},
    };

    for (const auto& s : table) {
        EXPECT_FALSE(Validator::validDate(s.first))
        << "Test case: " << s.first << "\tTest name: " << s.second;
    }
}

TEST(Validator_validLicense, TestPositive) {}

TEST(Validator_validLicense, TestNegative) {}

TEST(Validator_validLogin, TestPositive) {}

TEST(Validator_validLogin, TestNegative) {}

TEST(Validator_validPassword, TestPositive) {}

TEST(Validator_validPassword, TestNegative) {}

TEST(Validator_validPeriod, TestPositive) {}

TEST(Validator_validPeriod, TestNegative) {}

TEST(Validator_validAge, TestPositive) {}

TEST(Validator_validAge, TestNegative) {}

TEST(Validator_validDriver, TestPositive) {}

TEST(Validator_validDriver, TestNegavite) {}

TEST(Validator_validOrder, TestPositive) {}

TEST(Validator_validOrder, TestNegative) {}

TEST(Validator_validCar, TestPositive) {}

TEST(Validator_validCar, TestNegative) {}