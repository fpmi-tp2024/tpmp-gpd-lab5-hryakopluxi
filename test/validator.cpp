//
// Created by Stanislau Senkevich on 30.3.24.
//

#include <gtest/gtest.h>
#include "../include/validator.h"

TEST(Validator_validName, TestPositive) {
    std::string name = "Stanislau";
    EXPECT_TRUE(Validator::validName(name));
}

TEST(Validator_validName, TestNegative) {
    std::vector<std::pair<std::string, std::string>> table {
            {"", "Empty input"},
            {"Stas228", "Digits"},
            {"Stas-", "- character"},
            {"Stas!", "! character"},
            {"   Stas", "Odd spaces"},
            {"Stas Ivan", "Not a single word"}
    };

    for (const auto& s : table) {
        EXPECT_FALSE(Validator::validName(s.first))
        << "Test case: " << s.first << "\tTest name: " << s.second;
    }
}

TEST(Validator_validSurname, TestPositive) {
    std::string surname = "Ivanov";
    EXPECT_TRUE(Validator::validSurname(surname));
}

TEST(Validator_validSurname, TestNegative) {
    std::vector<std::pair<std::string, std::string>> table {
            {"", "Empty input"},
            {"Ivanov100", "Digits"},
            {"Ivanov-", "- character"},
            {"Ivanov!", "! character"},
            {"   Ivanov", "Odd spaces"},
            {"Ivanov Ivanov", "Not a single word"}
    };

    for (const auto& s : table) {
        EXPECT_FALSE(Validator::validSurname(s.first))
        << "Test case: " << s.first << "\tTest surname: " << s.second;
    }
}

TEST(Validator_validAddress, TestPositive) {
    std::string address = "Ulica 5";
    EXPECT_TRUE(Validator::validAddress(address));
}

TEST(Validator_validAddress, TestNegative) {
    std::vector<std::pair<std::string, std::string>> table {
            {"", "Empty input"},
            {"Ulica5", "Odd spaces"},
            {"   Ulica", "Odd spaces"},
            {"Ulica?", "? character"},
    };

    for (const auto& s : table) {
        EXPECT_FALSE(Validator::validAddress(s.first))
        << "Test case: " << s.first << "\tTest address: " << s.second;
    }
}

TEST(Validator_validCity, TestPositive) {
    std::string city = "Minsk";
    EXPECT_TRUE(Validator::validCity(city));
}

TEST(Validator_validCity, TestNegative) {
    std::vector<std::pair<std::string, std::string>> table {
            {"", "Empty input"},
            {"Minsk100", "Digit"},
            {"   Minsk", "Odd spaces"},
            {"Minsk?", "? character"},
    };

    for (const auto& s : table) {
        EXPECT_FALSE(Validator::validCity(s.first))
        << "Test case: " << s.first << "\tTest city: " << s.second;
    }
}

TEST(Validator_validDate, TestPositive) {
    std::string date = "2000-01-01";
    EXPECT_TRUE(Validator::validDate(date));
}

TEST(Validator_validDate, TestNegative) {
    std::vector<std::pair<std::string, std::string>> table {
            {"", "Empty input"},
            {"12-12-2004", "Invalid pattern"},
            {"1999-12-12", "Invalid year"},
            {"2024-13-24", "Invalid month"},
            {"2024-02-30", "Invalid month day"},
            {"2023-02-29", "Invalid month day"},
            {"2024-01-32", "Invalid month day"},
            {"2020 01 01", "Invalid pattern"}
    };

    for (const auto& s : table) {
        EXPECT_FALSE(Validator::validDate(s.first))
        << "Test case: " << s.first << "\tTest date: " << s.second;
    }
}

TEST(Validator_validLicense, TestPositive) {
    std::string license = "1234AA-1";
    EXPECT_TRUE(Validator::validLicense(license));
}

TEST(Validator_validLicense, TestNegative) {
    std::vector<std::pair<std::string, std::string>> table {
            {"", "Empty input"},
            {"0001AR-1", "Invalid literal part"},
            {"000011AA-1", "Invalid pattern"},
            {"1111AA-9", "Invalid city number"},
            {"9-1111AA", "Invalid pattern"},
            {"0000AA-1", "Invalid numeric part"}
    };

    for (const auto& s : table) {
        EXPECT_FALSE(Validator::validLicense(s.first))
        << "Test case: " << s.first << "\tTest license: " << s.second;
    }
}

TEST(Validator_validLogin, TestPositive) {
    std::vector<std::pair<std::string, std::string>> table {
            {"user123", "Numeric part possible"},
            {"user", "Literal login"},
            {"user_123", "_ character"},
    };

    for (const auto& s : table) {
        EXPECT_TRUE(Validator::validLogin(s.first))
        << "Test case: " << s.first << "\tTest login: " << s.second;
    }
}

TEST(Validator_validLogin, TestNegative) {
    std::vector<std::pair<std::string, std::string>> table {
            {"", "Empty input"},
            {"user 123", "Spaces impossible"},
            {" user", "Spaces impossible"},
            {"user/", "/ character"},
            {"user-123", "- character"},
            {"user?", "? character"},
            {"user!", "! character"},
            {"user.", ". character"},
            {"user+", "+ character"},
            {"user=", "= character"},
            {"user@", "@ character"},
            {"user$", "$ character"}
    };

    for (const auto& s : table) {
        EXPECT_FALSE(Validator::validLogin(s.first))
        << "Test case: " << s.first << "\tTest login: " << s.second;
    }
}

TEST(Validator_validPassword, TestPositive) {
    std::vector<std::pair<std::string, std::string>> table {
            {"user123", "Numeric part possible"},
            {"user", "Literal password"},
            {"user_123", "_ character"},
    };

    for (const auto& s : table) {
        EXPECT_TRUE(Validator::validPassword(s.first))
        << "Test case: " << s.first << "\tTest password: " << s.second;
    }
}

TEST(Validator_validPassword, TestNegative) {
    std::vector<std::pair<std::string, std::string>> table {
            {"", "Empty input"},
            {"user 123", "Spaces impossible"},
            {" user", "Spaces impossible"},
            {"user/", "/ character"},
            {"user-123", "- character"},
            {"user?", "? character"},
            {"user!", "! character"},
            {"user.", ". character"},
            {"user+", "+ character"},
            {"user=", "= character"},
            {"user@", "@ character"},
            {"user$", "$ character"}
    };

    for (const auto& s : table) {
        EXPECT_FALSE(Validator::validPassword(s.first))
        << "Test case: " << s.first << "\tTest password: " << s.second;
    }
}

TEST(Validator_validPeriod, TestPositive) {
    std::pair<std::string, std::string> period = {"2000-01-01", "2020-01-01"};
    EXPECT_TRUE(Validator::validPeriod(period.first, period.second));
}

TEST(Validator_validPeriod, TestNegative) {
    std::vector<std::pair<std::pair<std::string, std::string>, std::string>> table {
        {{"2024-01-00", "2024-01-02"}, "Invalid date"},
        {{"2024-01-01", "2024-01-00"}, "Invalid date"},
        {{"2024-13-01", "2025-01-02"}, "Invalid date"},
        {{"2024-01-00", "2024-13-02"}, "Invalid date"},
        {{"2023-02-29", "2024-01-02"}, "Invalid date"},
        {{"2023-02-28", "2024-01-32"}, "Invalid date"},
        {{"20-02-2024", "2024-01-02"}, "Invalid date pattern"},
        {{"2024 02 01", "2024-01-02"}, "Invalid date pattern"},
        {{"2024-02-01", "2024 march 2"}, "Invalid date pattern"},
        {{"", ""}, "Empty input"},
        {{"2026-02-21", "2024-01-02"}, "Invalid period"}, 
    };

    for (const auto& s : table) {
        EXPECT_FALSE(Validator::validPeriod(s.first.first, s.first.second))
        << "Test case: " << s.first.first << " - " << s.first.second << "\tTest period: " << s.second;
    }
}

TEST(Validator_validAge, TestPositive) {
    std::string age = "2000-01-01";
    EXPECT_TRUE(Validator::validAge(age));
}

TEST(Validator_validAge, TestNegative) {
    std::vector<std::pair<std::string, std::string>> table {
            {"", "Empty input"},
            {"12-12-2004", "Invalid pattern"},
            {"1999-12-12", "Invalid year"},
            {"2024-13-24", "Invalid month"},
            {"2024-02-30", "Invalid month day"},
            {"2023-02-29", "Invalid month day"},
            {"2024-01-32", "Invalid month day"},
            {"2020 01 01", "Invalid pattern"},
            {"2025-01-01", "Date is over current date"}
    };

    for (const auto& s : table) {
        EXPECT_FALSE(Validator::validAge(s.first))
        << "Test case: " << s.first << "\tTest date: " << s.second;
    }
}

TEST(Validator_validDriver, TestPositive) {
}

TEST(Validator_validDriver, TestNegavite) {}

TEST(Validator_validOrder, TestPositive) {}

TEST(Validator_validOrder, TestNegative) {}

TEST(Validator_validCar, TestPositive) {}

TEST(Validator_validCar, TestNegative) {}