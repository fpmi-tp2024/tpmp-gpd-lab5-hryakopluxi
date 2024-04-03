#include <gtest/gtest.h>
#include "../include/controller.h"

TEST(Controller_testConnection, TestPositive) {
    EXPECT_NO_THROW(Controller controller("db/app.db"));
    EXPECT_NO_THROW(Controller controller);
}

TEST(Controller_testConnection, TestNegative) {
    EXPECT_ANY_THROW(Controller controller("db/notapp.db"));
}

TEST(Controller_login, TestPositive) {
    Controller controller;
    std::vector<std::tuple<std::string, std::string, std::string>> table{
            {"admin",     "admin",     "Admin sign-in"},
            {"driver123", "driver123", "Driver sign-in"},
            {"disp100",   "disp100",   "Dispatcher sign-in"},
    };

    for (const auto& [login, password, testName]: table) {
        EXPECT_TRUE(controller.login(login, password))
                            << "Test case: " << login << " " << password << "\tTest name: " << testName;
    }
}

TEST(Controller_login, TestNegative) {
    Controller controller;
    std::vector<std::tuple<std::string, std::string, std::string>> table{
            {"admin",     "admin123",     "Admin sign-in"},
            {"driver", "driver123", "Driver sign-in"},
            {"disp",   "disp100",   "Dispatcher sign-in"},
    };

    for (const auto& [login, password, testName]: table) {
        EXPECT_FALSE(controller.login(login, password))
                            << "Test case: " << login << " " << password << "\tTest name: " << testName;
    }
}

TEST(Controller_getDriverOrders, TestPositive) {
    Controller controller;
    std::vector<std::tuple<std::pair<std::string, std::string>, int, std::pair<std::string, std::string>, std::vector<Order>, std::string>> table{
        {{"driver120", "driver120"}, 30, {"2020-01-01", "2029-05-05"}, {Order(8, 30, 10, "2024-09-08", 500, 2999, 120, 1), Order(17, 30, 10, "2025-06-03", 800, 2900, 138, 1)}, "Driver login"},
        {{"driver555", "driver555"}, 36, {"2020-01-01", "2029-05-05"}, {}, "Driver login with empty orders"},
        {{"disp234", "disp234"}, 29, {"2020-01-01", "2029-05-05"}, {Order(11, 29, 8, "2024-04-19", 400, 1800, 105, 1)}, "Dispatcher login"},
        {{"disp234", "disp234"}, 29, {"2020-01-01", "2020-05-05"}, {}, "Dispatcher login with no orders for the period"},
    };
    for (const auto& [user, id, period, orders, testName]: table) {
        EXPECT_NO_THROW({
            controller.login(user.first, user.second);
            auto controllerOrders = controller.getDriverOrders(id, period.first, period.second);

            EXPECT_EQ(orders.size(), controllerOrders.size()) << "Size of orders vectors does not match";

            for (int i = 0; i < std::min(orders.size(), controllerOrders.size()); i++) {
                EXPECT_TRUE(orders[i] == controllerOrders[i]) << "Order at index " << i << " does not match";
            }
        }) << "Test case: " << user.first << "\t" << user.second << "\tDriver: " << id << "\t" << period.first << " - " << period.second << "\tTest name: " << testName;
    }
}

TEST(Controller_getDriverOrders, TestNegative) {
    Controller controller;
    std::vector<std::tuple<std::pair<std::string, std::string>, int, std::pair<std::string, std::string>, std::vector<Order>, std::string>> table{
        {{"driver123", "driver123"}, 27, {"2020-01-01", "2024-05-05"}, {}, "Permission denied for driver"},
        {{"driver123", "driver123"}, 1, {"2020-01-01", "2024-05-05"}, {}, "Driver is not found"},
        {{"disp234", "disp234"}, 27, {"2020-01-01", "2024-05-05"}, {}, "Permission denied (cities are different)"},
        {{"disp555", "disp555"}, 29, {"2029-01-01", "2024-05-05"}, {}, "Invalid period"},
        {{"disp234", "disp234"}, 29, {"2020-01-01", "2029-05-05"}, {Order(11, 29, 8, "2024-04-19", 400, 1000, 105, 1)}, "Invalid orders"},
    };

    for (const auto& [user, id, period, orders, testName]: table) {
        EXPECT_ANY_THROW(
            try {
                controller.login(user.first, user.second);
                auto controllerOrders = controller.getDriverOrders(id, period.first, period.second);
                for (int i = 0; i < std::min(orders.size(), controllerOrders.size()); i++) {
                    if(!(orders[i] == controllerOrders[i])) {
                        throw std::invalid_argument("Orders are different");
                    }   
                }
            }
            catch(const PermissionDeniedException& e) {
                throw;
            }
            catch(const std::invalid_argument& e) {
                throw;
            }
        )
        << "Test case: " << user.first << "\t" << user.second << "\tDriver: " << id << "\t" << period.first << " - " << period.second << "\tTest name: " << testName;
    }
}

TEST(Controller_getCarSummaryMileageAndLoad, TestPositive) {}

TEST(Controller_getCarSummaryMileageAndLoad, TestNegative) {}

TEST(Controller_getDriverStatistics, TestPositive) {}

TEST(Controller_getDriverStatistics, TestNegative) {}

TEST(Controller_getAllDriversStatistics, TestPositive) {}

TEST(Controller_getAllDriversStatistics, TestNegative) {}

TEST(Controller_getWorstDriverStatistics, TestPositive) {}

TEST(Controller_getWorstDriverStatistics, TestNegative) {}

TEST(Controller_getInfoAboutCarWithMaxMileage, TestPositive) {}

TEST(Controller_getInfoAboutCarWithMaxMileage, TestNegative) {}

TEST(Controller_storeDriversEarnedMoney, TestPositive) {}

TEST(Controller_storeDriversEarnedMoney, TestNegative) {}

TEST(Controller_getDriverEarnedMoney, TestPositive) {}

TEST(Controller_getDriverEarnedMoney, TestNegative) {}

TEST(Controller_addCar, TestPositive) {}

TEST(Controller_addCar, TestNegative) {}

TEST(Controller_addDriver, TestPositive) {}

TEST(Controller_addDriver, TestNegative) {}

TEST(Controller_addOrder, TestPositive) {}

TEST(Controller_addOrder, TestNegative) {}

TEST(Controller_addDispatcher, TestPositive) {}

TEST(Controller_addDispatcher, TestNegative) {}

TEST(Controller_updateCar, TestPositive) {}

TEST(Controller_updateCar, TestNegative) {}

TEST(Controller_updateDriver, TestPositive) {}

TEST(Controller_updateDriver, TestNegative) {}

TEST(Controller_updateDispatcher, TestPositive) {}

TEST(Controller_updateDispatcher, TestNegative) {}

TEST(Controller_updateOrder, TestPositive) {}

TEST(Controller_updateOrder, TestNegative) {}

TEST(Controller_updateUser, TestPositive) {}

TEST(Controller_updateUser, TestNegative) {}

TEST(Controller_updateOrderApproveStatus, TestPositive) {}

TEST(Controller_updateOrderApproveStatus, TestNegative) {}

TEST(Controller_deleteCar, TestPositive) {}

TEST(Controller_deleteCar, TestNegative) {}

TEST(Controller_deleteDriver, TestPositive) {}

TEST(Controller_deleteDriver, TestNegative) {}

TEST(Controller_deleteOrder, TestPositive) {}

TEST(Controller_deleteOrder, TestNegative) {}

TEST(Controller_deleteDispatcher, TestPositive) {}

TEST(Controller_deleteDispatcher, TestNegative) {}



