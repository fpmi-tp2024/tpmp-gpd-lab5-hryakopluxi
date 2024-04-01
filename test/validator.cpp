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
        << "Test case: " << s.first << "\tTest name: " << s.second;
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
        << "Test case: " << s.first << "\tTest name: " << s.second;
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
        << "Test case: " << s.first << "\tTest name: " << s.second;
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
        << "Test case: " << s.first << "\tTest name: " << s.second;
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
        << "Test case: " << s.first << "\tTest name: " << s.second;
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
        << "Test case: " << s.first << "\tTest name: " << s.second;
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
        << "Test case: " << s.first << "\tTest name: " << s.second;
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
        << "Test case: " << s.first << "\tTest name: " << s.second;
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
        << "Test case: " << s.first << "\tTest name: " << s.second;
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
        << "Test case: " << s.first.first << " - " << s.first.second << "\tTest name: " << s.second;
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
        << "Test case: " << s.first << "\tTest name: " << s.second;
    }
}

TEST(Validator_validDriver, TestPositive) {
    Driver driver("driver123", "driver123", "Stas", "Senkevich", {Category::A}, 1, "Ulica 5", "Minsk", "2004-05-18");
    EXPECT_NO_THROW(
        EXPECT_TRUE(Validator::validDriver(driver));
    );
}

TEST(Validator_validDriver, TestNegative) {
    std::vector<std::pair<const Driver&, std::string>> table {
        {Driver("", "password123", "Stas", "Senkevich", {Category::A}, 1, "Ulica 5", "Minsk", "2004-05-18"), "Empty login"},
        {Driver("driver123", "password123", "", "Senkevich", {Category::A}, 1, "Ulica 5", "Minsk", "2004-05-18"), "Empty name"},
        {Driver("driver123", "", "Stas", "Senkevich", {Category::A}, 1, "Ulica 5", "Minsk", "2004-05-18"), "Empty password"},
        {Driver("driver123", "password123", "Stas", "", {Category::A}, 1, "Ulica 5", "Minsk", "2004-05-18"), "Empty surname"},
        {Driver("driver123", "password123", "Stas", "Senkevich", {Category::A}, 1, "", "Minsk", "2004-05-18"), "Empty address"},
        {Driver("driver123", "password123", "Stas", "Senkevich", {Category::A}, 1, "Ulica 5", "", "2004-05-18"), "Empty city"},
        {Driver("driver123", "password123", "Stas", "Senkevich", {Category::A}, 1, "Ulica 5", "Minsk", ""), "Empty birthday"},

        {Driver("driver 123", "password123", "Stas", "Senkevich", {Category::A}, 1, "Ulica 5", "Minsk", "2004-05-18"), "Invalid login (spaces are impossible)"},
        {Driver("driver@123", "password123", "Stas", "Senkevich", {Category::A}, 1, "Ulica 5", "Minsk", "2004-05-18"), "Invalid login (@ character)"},
        {Driver("driver!", "password123", "Stas", "Senkevich", {Category::A}, 1, "Ulica 5", "Minsk", "2004-05-18"), "Invalid login(! character)"},

        {Driver("driver123", "password 123", "Stas", "Senkevich", {Category::A}, 1, "Ulica 5", "Minsk", "2004-05-18"), "Invalid password (spaces are impossible)"},
        {Driver("driver123", "password@123", "Stas", "Senkevich", {Category::A}, 1, "Ulica 5", "Minsk", "2004-05-18"), "Invalid password (@ character)"},
        {Driver("driver123", "1234=", "Stas", "Senkevich", {Category::A}, 1, "Ulica 5", "Minsk", "2004-05-18"), "Invalid password (= character)"},

        {Driver("driver123", "password123", "Stas5", "Senkevich", {Category::A}, 1, "Ulica 5", "Minsk", "2004-05-18"), "Invalid name (contains digit)"},
        {Driver("driver123", "password123", "Stas stas", "Senkevich", {Category::A}, 1, "Ulica 5", "Minsk", "2004-05-18"), "Invalid name (not a single word)"},

        {Driver("driver123", "password123", "Stas", "Senkevich5", {Category::A}, 1, "Ulica 5", "Minsk", "2004-05-18"), "Invalid surname (contains digit)"},
        {Driver("driver123", "password123", "Stas", "Senkevich stas", {Category::A}, 1, "Ulica 5", "Minsk", "2004-05-18"), "Invalid surname (not a single word)"},

        {Driver("driver123", "password123", "Stas", "Senkevich", {Category::A}, 1, "Ulica5", "Minsk", "2004-05-18"), "Invalid address (not a single word)"},
        {Driver("driver123", "password123", "Stas", "Senkevich", {Category::A}, 1, "   Ulica", "Minsk", "2004-05-18"), "Invalid address (odd spaces)"},
        
        {Driver("driver123", "password123", "Stas", "Senkevich", {Category::A}, 1, "Ulica 5", "Minsk yuzhniy", "2004-05-18"), "Invalid city (not a single word)"},
        {Driver("driver123", "password123", "Stas", "Senkevich", {Category::A}, 1, "Ulica 5", "Minsk 10", "2004-05-18"), "Invalid city (digits are impossible)"},

        {Driver("driver123", "password123", "Stas", "Senkevich", {Category::A}, 1, "Ulica 5", "Minsk", "18-05-2004"), "Invalid birthday pattern"},
        {Driver("driver123", "password123", "Stas", "Senkevich stas", {Category::A}, 1, "Ulica5", "Minsk", "05-2004-18"), "Invalid birthday pattern"},
        {Driver("driver123", "password123", "Stas", "Senkevich stas", {Category::A}, 1, "Ulica5", "Minsk", "2004-05-32"), "Invalid birthday pattern"},
        {Driver("driver123", "password123", "Stas", "Senkevich stas", {Category::A}, 1, "Ulica5", "Minsk", "2004-13-30"), "Invalid birthday pattern"},
        {Driver("driver123", "password123", "Stas", "Senkevich stas", {Category::A}, 1, "Ulica5", "Minsk", "2023-02-29"), "Invalid birthday pattern"},
    };

    for (const auto& [driver, testName] : table) {
        EXPECT_THROW(
            {
                try {
                    Validator::validDriver(driver);
                } catch (const std::invalid_argument& e) {
                    throw;
                }
            }, 
            std::invalid_argument)
            << "Test case: " << testName << driver.print();
    }
}

TEST(Validator_validDispatcher, TestPositive) {
    Dispatcher dispatcher("disp123", "disp123", "Stas", "Senkevich", "Ulica 5", "Minsk");
    EXPECT_NO_THROW(
        EXPECT_TRUE(Validator::validDispatcher(dispatcher));
    );
}

TEST(Validator_validDispatcher, TestNegative) {
    std::vector<std::pair<const Dispatcher&, std::string>> table {
        {Dispatcher("", "disp123", "Yulia", "Raitsyna", "Ulica 5", "Minsk"), "Empty login"},
        {Dispatcher("disp123", "", "Yulia", "Raitsyna", "Ulica 5", "Minsk"), "Empty password"},
        {Dispatcher("disp123", "disp123", "", "Raitsyna", "Ulica 5", "Minsk"), "Empty name"},
        {Dispatcher("disp123", "disp123", "Yulia", "", "Ulica 5", "Minsk"), "Empty surname"},
        {Dispatcher("disp123", "disp123", "Yulia", "Raitsyna", "", "Minsk"), "Empty address"},
        {Dispatcher("disp123", "disp123", "Yulia", "Raitsyna", "Ulica 5", ""), "Empty city"},

        {Dispatcher("disp 123", "disp123", "Yulia", "Raitsyna", "Ulica 5", "Minsk"), "Invalid login (spaces are impossible)"},
        {Dispatcher("disp@123", "disp123", "Yulia", "Raitsyna", "Ulica 5", "Minsk"), "Invalid login (@ character)"},
        {Dispatcher("123!", "disp123", "Yulia", "Raitsyna", "Ulica 5", "Minsk"), "Invalid login (! character)"},

        {Dispatcher("disp123", "disp 123", "Yulia", "Raitsyna", "Ulica 5", "Minsk"), "Invalid lpassword (spaces are impossible)"},
        {Dispatcher("disp123", "disp1@23", "Yulia", "Raitsyna", "Ulica 5", "Minsk"), "Invalid password (@ character)"},
        {Dispatcher("disp123", "disp!", "Yulia", "Raitsyna", "Ulica 5", "Minsk"), "Invalid password (! character)"},

        {Dispatcher("disp123", "disp123", "Yulia yulia", "Raitsyna", "Ulica 5", "Minsk"), "Invalid name (not a single word)"},
        {Dispatcher("disp123", "disp123", "Yulia@", "Raitsyna", "Ulica 5", "Minsk"), "Invalid name (@ character)"},

        {Dispatcher("disp123", "disp123", "Yulia", "Raitsyna yulia", "Ulica 5", "Minsk"), "Invalid surname (not a single word)"},
        {Dispatcher("disp123", "disp123", "Yulia", "Raitsyna5", "Ulica 5", "Minsk"), "Invalid surname (digits are impossible)"},

        {Dispatcher("disp123", "disp123", "Yulia", "Raitsyna", "Ulica5", "Minsk"), "Invalid address pattern"},
        {Dispatcher("disp123", "disp123", "Yulia", "Raitsyna", "   Ulica ", "Minsk"), "Invalid addres (odd spaces)"},

        {Dispatcher("disp123", "disp123", "Yulia", "Raitsyna", "Ulica 5", "Minsk moskovskiy"), "Invalid city (not a single word)"},
        {Dispatcher("disp123", "disp123", "Yulia", "Raitsyna", "Ulica 5", "Minsk5"), "Invalid city (digits are impossible)"},

    };

    for (const auto& [dispatcher, testName] : table) {
        EXPECT_THROW(
            {
                try {
                    Validator::validDispatcher(dispatcher);
                } catch (const std::invalid_argument& e) {
                    throw;
                }
            }, 
            std::invalid_argument)
            << "Test case: " << testName << dispatcher.print();
    }
}

TEST(Validator_validOrder, TestPositive) {
    Order order(1, 27, 7, "2024-01-01", 200, 1000, 120, true);
    const std::string db_filename = "db/app.db";
    sqlite3* db;
    int rc = sqlite3_open_v2(db_filename.c_str(), &db, SQLITE_OPEN_READWRITE, nullptr);
    if (rc != SQLITE_OK) {
        throw std::invalid_argument("Failed to open database");
    }

    EXPECT_NO_THROW({
        EXPECT_TRUE(Validator::validOrder(order, db));
    });
    sqlite3_close(db);
}

TEST(Validator_validOrder, TestNegative) {
    const std::string db_filename = "db/app.db";
    sqlite3* db = nullptr;
        int rc = sqlite3_open_v2(db_filename.c_str(), &db, SQLITE_OPEN_READWRITE, nullptr);
        if (rc != SQLITE_OK) {
            sqlite3_close_v2(db);
            throw std::runtime_error("Cannot open database");
        }

    std::vector<std::pair<Order, std::string>> table {
        {Order(1, 27, 7, "", 200, 1000, 120, true), "Empty date"},
        {Order(1, 27, 7, "01-01-2024", 200, 1000, 120, true), "Invalid date pattern"},
        {Order(1, 27, 7, "2024-13-01", 200, 1000, 120, true), "Invalid date (month)"},
        {Order(1, 27, 7, "2024-01-32", 200, 1000, 120, true), "Invalid date (day)"},
        {Order(1, 27, 7, "2023-02-29", 200, 1000, 120, true), "Invalid date"},
        {Order(1, 27, 7, "2024-01-01", 200, 1000, 120, true), "Not a driver's car"},
        {Order(1, 27, 7, "2024-01-01", -200, 1000, 120, true), "Negative mileage"},
        {Order(1, 27, 7, "2024-01-01", 200, -1000, 120, true), "Negative load"},
        {Order(1, 27, 7, "2024-01-01", 200, 1000, -120, true), "Negative cost"},
    };

    for (const auto& [order, testName] : table) {

        EXPECT_THROW({
            try {
                Validator::validOrder(order, db);
            } catch (const std::invalid_argument& e) {
                throw;
            }
        }, std::invalid_argument) << "Test case: " << testName << order.print();

        sqlite3_close(db);
    }
}


TEST(Validator_validCar, TestPositive) {
    Car car(1, 27, "1234AA-3", "volvo", 200, 2000);
    const std::string db_filename = "db/app.db";
    sqlite3* db;
    int rc = sqlite3_open_v2(db_filename.c_str(), &db, SQLITE_OPEN_READWRITE, nullptr);
    if (rc != SQLITE_OK) {
        throw std::invalid_argument("Failed to open database");
    }

    EXPECT_NO_THROW({
        EXPECT_TRUE(Validator::validCar(car, db));
    });
    sqlite3_close(db);
}

TEST(Validator_validCar, TestNegative) {

    const std::string db_filename = "db/app.db";
    sqlite3* db = nullptr;
    int rc = sqlite3_open_v2(db_filename.c_str(), &db, SQLITE_OPEN_READWRITE, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close_v2(db);
        throw std::runtime_error("Cannot open database");
    }

    std::vector<std::pair<Car, std::string>> table {
        {Car(1, 2, "1234AA-1", "volvo", 200, 2000), "No driver found in data base"},
        {Car(1, 27, "1234A-1", "volvo", 200, 2000), "Invalid license pattern"},
        {Car(1, 27, "1234AW-1", "volvo", 200, 2000),  "Invalid license pattern"},
        {Car(1, 27, "1234AA-9", "volvo", 200, 2000), "Invalid license pattern"},
        {Car(1, 27, "AA1234-1", "volvo", 200, 2000), "Invalid license pattern"},
        {Car(1, 27, "1234AA-1", "volvo", -200, 2000), "Invalid mileage (negative)"},
        {Car(1, 27, "1234AA-1", "volvo", 200, -2000), "Invalid capacity (negative)"}, 
    };

    for (const auto& [car, testName] : table) {        

        EXPECT_THROW({
            try {
                Validator::validCar(car, db);
            } catch (const std::invalid_argument& e) {
                throw;
            }
        }, std::invalid_argument) << "Test case: " << testName << car.print();

        sqlite3_close(db);
    }
}
