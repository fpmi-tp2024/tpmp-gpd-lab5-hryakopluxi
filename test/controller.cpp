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
    std::vector<std::vector<std::string>> table{
            {"admin",     "admin",     "Admin sign-in"},
            {"driver123", "driver123", "Driver sign-in"},
            {"disp100",   "disp100",   "Dispatcher sign-in"},
    };

    for (const auto& t: table) {
        EXPECT_TRUE(controller.login(t[0], t[1]))
                            << "Test case: " << t[0] << " " << t[1] << "\tTest name: " << t[2];
    }
}

TEST(Controller_login, TestNegative) {}

TEST(Controller_getDriverOrders, TestPositive) {}

TEST(Controller_getDriverOrders, TestNegative) {}

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



