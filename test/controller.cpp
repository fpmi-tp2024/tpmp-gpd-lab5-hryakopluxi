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

TEST(Controller_getCarSummaryMileageAndLoad, TestPositive) {
    Controller controller;
    std::vector<std::tuple<std::pair<std::string, std::string>, int, std::string, std::string>> table = {
        {{"driver123", "driver123"}, 4, "Summary load: 4850.000\nSummary mileage: 1450.0\n", "Driver login"},
        {{"disp", "disp"}, 4, "Summary load: 4850.000\nSummary mileage: 1450.0\n", "Dispatcher login"},
        {{"admin", "admin"}, 4, "Summary load: 4850.000\nSummary mileage: 1450.0\n", "Admin login"}
    };
    for (const auto& [user, car_id, summary, testName]: table) {
        EXPECT_NO_THROW(
            controller.login(user.first, user.second);
            std::string controllerSummary = controller.getCarSummaryMileageAndLoad(car_id);
            EXPECT_EQ(summary, controllerSummary) << "Test case: " << user.first << "\t" << user.second << "\tDriver: " << car_id << "\t" << "\tTest name: " << testName;
        )<< "Test case: " << user.first << "\t" << user.second << "\tDriver: " << car_id << "\t" << "\tTest name: " << testName;
    }  
}

TEST(Controller_getCarSummaryMileageAndLoad, TestNegative) {
    Controller controller;
    std::vector<std::tuple<std::pair<std::string, std::string>, int, std::string, std::string>> table = {
        {{"driver123", "driver123"}, 100, "Summary load: 4850.000\nSummary mileage: 1450.0\n", "No car found"},
        {{"driver123", "driver123"}, 1, "Summary load: 4850.000\nSummary mileage: 1450.0\n", "Not driver's car"},
        {{"disp", "disp"}, 100, "Summary load: 4850.000\nSummary mileage: 1450.0\n", "No car found"},
        {{"admin", "admin"}, 100, "Summary load: 4850.000\nSummary mileage: 1450.0\n", "No car found"},
        {{"driver123", "driver123"}, 4, "Summary load: 4000.000\nSummary mileage: 1450.0\n", "No car found"},
    };
    for (const auto& [user, car_id, summary, testName]: table) {
        EXPECT_ANY_THROW(
            try {
                controller.login(user.first, user.second);
                std::string controllerSummary = controller.getCarSummaryMileageAndLoad(car_id);
                if(summary != controllerSummary) {
                    throw std::invalid_argument("Summaries are not equal");
                }
            }
            catch(PermissionDeniedException& e) {
                throw;
            }
            catch(std::invalid_argument& e) {
                throw;
            }
        )<< "Test case: " << user.first << "\t" << user.second << "\tDriver: " << car_id << "\t" << "\tTest name: " << testName;
    } 
}

TEST(Controller_getDriverStatistics, TestPositive) {
    Controller controller;
    std::vector<std::tuple<std::pair<std::string, std::string>, int, std::string, std::string>> table = {
        {{"admin", "admin"}, 27,"ID: 27\n"
                                "Login: driver1337\n"
                                "Name: stanislau\n"
                                "Surname: senkevich\n"
                                "Category: A B C\n"
                                "Experience: 1 years\n"
                                "City: moscow\n"
                                "Address: u\n"
                                "Birthday: 2004-05-18\n"
                                "Orders completed: 4\n"
                                "Money earned: 134.00\n"
                                "Summary mileage: 2530.00\n",
                                                            "Admin login"},
        {{"driver1337", "driver1337"}, 27,"ID: 27\n"
                                "Login: driver1337\n"
                                "Name: stanislau\n"
                                "Surname: senkevich\n"
                                "Category: A B C\n"
                                "Experience: 1 years\n"
                                "City: moscow\n"
                                "Address: u\n"
                                "Birthday: 2004-05-18\n"
                                "Orders completed: 4\n"
                                "Money earned: 134.00\n"
                                "Summary mileage: 2530.00\n",
                                                            "Driver login"},
        {{"disp234", "disp234"}, 29,"ID: 29\n"
                                "Login: driver100\n"
                                "Name: alexandr\n"
                                "Surname: shishkin\n"
                                "Category: B C\n"
                                "Experience: 3 years\n"
                                "City: mogilev\n"
                                "Address: turova\n"
                                "Birthday: 1984-02-05\n"
                                "Orders completed: 1\n"
                                "Money earned: 21.00\n"
                                "Summary mileage: 400.00\n",
                                                            "Dispatcher login"},
        
    };
    for (const auto& [user, driver_id, statistics, testName]: table) {
        EXPECT_NO_THROW(
            controller.login(user.first, user.second);
            std::string controllerStatistics = controller.getDriverStatistics(driver_id);
            EXPECT_EQ(statistics, controllerStatistics) << "Test case: " << user.first << "\t" << user.second << "\tDriver: " << driver_id << "\t" << "\tTest name: " << testName;
        )<< "Test case: " << user.first << "\t" << user.second << "\tDriver: " << driver_id << "\t" << "\tTest name: " << testName;
    }  
}

TEST(Controller_getDriverStatistics, TestNegative) {
    Controller controller;
    std::vector<std::tuple<std::pair<std::string, std::string>, int, std::string, std::string>> table = {
        {{"admin", "admin"}, 90,"ID: 27\n"
                                "Login: driver1337\n"
                                "Name: stanislau\n"
                                "Surname: senkevich\n"
                                "Category: A B C\n"
                                "Experience: 1 years\n"
                                "City: moscow\n"
                                "Address: u\n"
                                "Birthday: 2004-05-18\n"
                                "Orders completed: 4\n"
                                "Money earned: 134.00\n"
                                "Summary mileage: 2530.00\n",
                                                            "Driver is not found"},
        {{"driver123", "driver123"}, 27,"ID: 27\n"
                                "Login: driver1337\n"
                                "Name: stanislau\n"
                                "Surname: senkevich\n"
                                "Category: A B C\n"
                                "Experience: 1 years\n"
                                "City: moscow\n"
                                "Address: u\n"
                                "Birthday: 2004-05-18\n"
                                "Orders completed: 4\n"
                                "Money earned: 134.00\n"
                                "Summary mileage: 2530.00\n",
                                                            "Driver login (permission denied)"},
        {{"disp100", "disp100"}, 27,"ID: 27\n"
                                "Login: driver1337\n"
                                "Name: stanislau\n"
                                "Surname: senkevich\n"
                                "Category: A B C\n"
                                "Experience: 1 years\n"
                                "City: moscow\n"
                                "Address: u\n"
                                "Birthday: 2004-05-18\n"
                                "Orders completed: 4\n"
                                "Money earned: 134.00\n"
                                "Summary mileage: 2530.00\n",
                                                            "Dispatcher login (permission denied)"},
    };
    for (const auto& [user, driver_id, statistics, testName]: table) {
        EXPECT_ANY_THROW( 
            try {
                controller.login(user.first, user.second);
                std::string controllerStatistics = controller.getDriverStatistics(driver_id);
            }
            catch(PermissionDeniedException& e) {
                throw;
            }
            catch(std::invalid_argument& e) {
                throw;
            }
        )<< "Test case: " << user.first << "\t" << user.second << "\tDriver: " << driver_id << "\t" << "\tTest name: " << testName;
    }  
}

TEST(Controller_getAllDriversStatistics, TestPositive) {
    Controller controller;
    std::vector<std::tuple<std::pair<std::string, std::string>, std::vector<std::string>, std::string>> table = {
        {{"admin", "admin"}, {
                          "ID: 27\n"
                          "Login: driver1337\n"
                          "Name: stanislau\n"
                          "Surname: senkevich\n"
                          "Category: A B C\n"
                          "Experience: 1 years\n"
                          "City: moscow\n"
                          "Address: u\n"
                          "Birthday: 2004-05-18\n"
                          "Orders completed: 4\n"
                          "Money earned: 670.00\n"
                          "Summary mileage: 2530.00\n",
                          "ID: 28\n"
                          "Login: driver123\n"
                          "Name: ivan\n"
                          "Surname: ivanov\n"
                          "Category: \n"
                          "Experience: 1 years\n"
                          "City: minsk\n"
                          "Address: nezavisimosty\n"
                          "Birthday: 2000-09-09\n"
                          "Orders completed: 5\n"
                          "Money earned: 691.00\n"
                          "Summary mileage: 2300.00\n",
                          "ID: 29\n"
                          "Login: driver100\n"
                          "Name: alexandr\n"
                          "Surname: shishkin\n"
                          "Category: B C\n"
                          "Experience: 3 years\n"
                          "City: mogilev\n"
                          "Address: turova\n"
                          "Birthday: 1984-02-05\n"
                          "Orders completed: 1\n"
                          "Money earned: 105.00\n"
                          "Summary mileage: 400.00\n",
                          "ID: 30\n"
                          "Login: driver120\n"
                          "Name: igor\n"
                          "Surname: kalinin\n"
                          "Category: B C\n"
                          "Experience: 6 years\n"
                          "City: borisov\n"
                          "Address: simonova\n"
                          "Birthday: 1987-06-23\n"
                          "Orders completed: 2\n"
                          "Money earned: 258.00\n"
                          "Summary mileage: 1300.00\n",
                          "ID: 31\n"
                          "Login: driver456\n"
                          "Name: vadim\n"
                          "Surname: borisov\n"
                          "Category: B C D\n"
                          "Experience: 10 years\n"
                          "City: orsha\n"
                          "Address: ponomorenko\n"
                          "Birthday: 1992-04-23\n"
                          "Orders completed: 3\n"
                          "Money earned: 590.00\n"
                          "Summary mileage: 2450.00\n",
                          "ID: 36\n"
                          "Login: driver555\n"
                          "Name: roman\n"
                          "Surname: romanov\n"
                          "Category: B\n"
                          "Experience: 2 years\n"
                          "City: moscow\n"
                          "Address: frunzenskaya\n"
                          "Birthday: 1994-03-17\n"
                          "Orders completed: 0\n"
                          "Money earned: 0.00\n"
                          "Summary mileage: 0.00\n"
                          },                            "Admin login"},
        {{"disp234", "disp234"}, {
                          "ID: 29\n"
                          "Login: driver100\n"
                          "Name: alexandr\n"
                          "Surname: shishkin\n"
                          "Category: B C\n"
                          "Experience: 3 years\n"
                          "City: mogilev\n"
                          "Address: turova\n"
                          "Birthday: 1984-02-05\n"
                          "Orders completed: 1\n"
                          "Money earned: 105.00\n"
                          "Summary mileage: 400.00\n",
                          },                            "Dispatcher login"},
    };
    for (const auto& [user, statistics, testName]: table) {
        EXPECT_NO_THROW( 
            controller.login(user.first, user.second);
            std::vector<std::string> controllerStatistics = controller.getAllDriversStatistics();
            EXPECT_EQ(statistics.size(), controllerStatistics.size()) << "Test name: " << testName;
            for(int i = 0; i < std::min(statistics.size(), controllerStatistics.size()); i++) {
                EXPECT_EQ(statistics[i], controllerStatistics[i])<< "Test case: " << user.first << "\t" << user.second << "Statistics: "<< statistics[i] << "\t" << "\tTest name: " << testName;
            }
        )<< "Test case: " << user.first << "\t" << user.second << "\t" << "\tTest name: " << testName;
    }  
}

TEST(Controller_getAllDriversStatistics, TestNegative) {
    Controller controller;
    std::vector<std::tuple<std::pair<std::string, std::string>, std::vector<std::string>, std::string>> table = {
        {{"driver123", "driver123"}, {}, "Permission denied for driver"},
        {{"admin", "admin"}, {
                          "ID: 28\n"
                          "Login: driver123\n"
                          "Name: ivan\n"
                          "Surname: ivanov\n"
                          "Category: \n"
                          "Experience: 1 years\n"
                          "City: minsk\n"
                          "Address: nezavisimosty\n"
                          "Birthday: 2000-09-09\n"
                          "Orders completed: 5\n"
                          "Money earned: 691.00\n"
                          "Summary mileage: 2300.00\n",
                          "ID: 29\n"
                          "Login: driver100\n"
                          "Name: alexandr\n"
                          "Surname: shishkin\n"
                          "Category: B C\n"
                          "Experience: 3 years\n"
                          "City: mogilev\n"
                          "Address: turova\n"
                          "Birthday: 1984-02-05\n"
                          "Orders completed: 1\n"
                          "Money earned: 105.00\n"
                          "Summary mileage: 400.00\n",
                          "ID: 30\n"
                          "Login: driver120\n"
                          "Name: igor\n"
                          "Surname: kalinin\n"
                          "Category: B C\n"
                          "Experience: 6 years\n"
                          "City: borisov\n"
                          "Address: simonova\n"
                          "Birthday: 1987-06-23\n"
                          "Orders completed: 2\n"
                          "Money earned: 258.00\n"
                          "Summary mileage: 1300.00\n",
                          "ID: 31\n"
                          "Login: driver456\n"
                          "Name: vadim\n"
                          "Surname: borisov\n"
                          "Category: B C D\n"
                          "Experience: 10 years\n"
                          "City: orsha\n"
                          "Address: ponomorenko\n"
                          "Birthday: 1992-04-23\n"
                          "Orders completed: 3\n"
                          "Money earned: 590.00\n"
                          "Summary mileage: 2450.00\n",
                          "ID: 36\n"
                          "Login: driver555\n"
                          "Name: roman\n"
                          "Surname: romanov\n"
                          "Category: B\n"
                          "Experience: 2 years\n"
                          "City: moscow\n"
                          "Address: frunzenskaya\n"
                          "Birthday: 1994-03-17\n"
                          "Orders completed: 0\n"
                          "Money earned: 0.00\n"
                          "Summary mileage: 0.00\n"
                          },                            "Admin login(not same number of orders)"},
        {{"admin", "admin"}, {
                          "ID: 27\n"
                          "Login: driver1337\n"
                          "Name: stanislau\n"
                          "Surname: senkevich\n"
                          "Category: A B C\n"
                          "Experience: 1 years\n"
                          "City: moscow\n"
                          "Address: u\n"
                          "Birthday: 2004-05-18\n"
                          "Orders completed: 4\n"
                          "Money earned: 670.00\n"
                          "Summary mileage: 2530.00\n",
                          "ID: 200000\n"
                          "Login: driver123\n"
                          "Name: ivan\n"
                          "Surname: ivanov\n"
                          "Category: \n"
                          "Experience: 1 years\n"
                          "City: minsk\n"
                          "Address: nezavisimosty\n"
                          "Birthday: 2000-09-09\n"
                          "Orders completed: 5\n"
                          "Money earned: 691.00\n"
                          "Summary mileage: 2300.00\n",
                          "ID: 29\n"
                          "Login: driver100\n"
                          "Name: alexandr\n"
                          "Surname: shishkin\n"
                          "Category: B C\n"
                          "Experience: 3 years\n"
                          "City: mogilev\n"
                          "Address: turova\n"
                          "Birthday: 1984-02-05\n"
                          "Orders completed: 1\n"
                          "Money earned: 105.00\n"
                          "Summary mileage: 400.00\n",
                          "ID: 30\n"
                          "Login: driver120\n"
                          "Name: igor\n"
                          "Surname: kalinin\n"
                          "Category: B C\n"
                          "Experience: 6 years\n"
                          "City: borisov\n"
                          "Address: simonova\n"
                          "Birthday: 1987-06-23\n"
                          "Orders completed: 2\n"
                          "Money earned: 258.00\n"
                          "Summary mileage: 1300.00\n",
                          "ID: 31\n"
                          "Login: driver456\n"
                          "Name: vadim\n"
                          "Surname: borisov\n"
                          "Category: B C D\n"
                          "Experience: 10 years\n"
                          "City: orsha\n"
                          "Address: ponomorenko\n"
                          "Birthday: 1992-04-23\n"
                          "Orders completed: 3\n"
                          "Money earned: 590.00\n"
                          "Summary mileage: 2450.00\n",
                          "ID: 36\n"
                          "Login: driver555\n"
                          "Name: roman\n"
                          "Surname: romanov\n"
                          "Category: B\n"
                          "Experience: 2 years\n"
                          "City: moscow\n"
                          "Address: frunzenskaya\n"
                          "Birthday: 1994-03-17\n"
                          "Orders completed: 0\n"
                          "Money earned: 0.00\n"
                          "Summary mileage: 0.00\n"
                          },                            "Admin login(wrong data)"},
    };
    for (const auto& [user, statistics, testName]: table) {
        EXPECT_ANY_THROW( 
            try {
                controller.login(user.first, user.second);
                std::vector<std::string> controllerStatistics = controller.getAllDriversStatistics();
                for(int i = 0; i < std::min(statistics.size(), controllerStatistics.size()); i++) {
                    if(statistics[i] != controllerStatistics[i]) throw std::invalid_argument("Not equal data");
                }       
            }
            catch(PermissionDeniedException& e) {
                throw;
            }
            catch(std::invalid_argument& e){
                throw;
            }
            
        )<< "Test case: " << user.first << "\t" << user.second << "\t" << "\tTest name: " << testName;
    }
}

TEST(Controller_getWorstDriverStatistics, TestNegative) {
    Controller controller;
    std::vector<std::tuple<std::pair<std::string, std::string>, std::string, std::string>> table = {
        {{"admin", "admin"},    "ID: 31\n"
                                "Login: driver456\n"
                                "Name: vadim\n"
                                "Surname: borisov\n"
                                "Category: B C D\n"
                                "Experience: 10 years\n"
                                "City: orsha\n"
                                "Address: ponomorenko\n"
                                "Birthday: 1992-04-23\n"
                                "Orders completed: 3\n"
                                "Money earned: 590.00\n"
                                "Summary mileage: 2450.00\n",
                                                              "Admin login (wrong driver)"},
        {{"driver123", "driver123"},"ID: 36\n"
                                "Login: driver555\n"
                                "Name: roman\n"
                                "Surname: romanov\n"
                                "Category: B\n"
                                "Experience: 2 years\n"
                                "City: moscow\n"
                                "Address: frunzenskaya\n"
                                "Birthday: 1994-03-17\n"
                                "Orders completed: 0\n"
                                "Money earned: 0.00\n"
                                "Summary mileage: 0.00\n",
                                                            "Driver login(permission denied)"},
        
    };
    for (const auto& [user, statistics, testName]: table) {
        EXPECT_ANY_THROW(
            try {
                controller.login(user.first, user.second);
                std::string controllerStatistics = controller.getWorstDriverStatistics();
                if(statistics != controllerStatistics) {
                    throw std::invalid_argument("Data is wrong");
                }
            }
            catch(PermissionDeniedException& e) {
                throw;
            }
            catch(std::invalid_argument& e) {
                throw;
            }
        )<< "Test case: " << user.first << "\t" << user.second << "\tData: " << statistics << "\t" << "\tTest name: " << testName;
    }  
}

TEST(Controller_getWorstDriverStatistics, TestPositive) {
    Controller controller;
    std::vector<std::tuple<std::pair<std::string, std::string>, std::string, std::string>> table = {
        {{"admin", "admin"},    "ID: 36\n"
                                "Login: driver555\n"
                                "Name: roman\n"
                                "Surname: romanov\n"
                                "Category: B\n"
                                "Experience: 2 years\n"
                                "City: moscow\n"
                                "Address: frunzenskaya\n"
                                "Birthday: 1994-03-17\n"
                                "Orders completed: 0\n"
                                "Money earned: 0.00\n"
                                "Summary mileage: 0.00\n",
                                                            "Admin login"},
        {{"disp234", "disp234"},"ID: 36\n"
                                "Login: driver555\n"
                                "Name: roman\n"
                                "Surname: romanov\n"
                                "Category: B\n"
                                "Experience: 2 years\n"
                                "City: moscow\n"
                                "Address: frunzenskaya\n"
                                "Birthday: 1994-03-17\n"
                                "Orders completed: 0\n"
                                "Money earned: 0.00\n"
                                "Summary mileage: 0.00\n",
                                                            "Dispatcher login"},
        
    };
    for (const auto& [user, statistics, testName]: table) {
        EXPECT_NO_THROW(
            controller.login(user.first, user.second);
            std::string controllerStatistics = controller.getWorstDriverStatistics();
            EXPECT_EQ(statistics, controllerStatistics) << "Test case: " << user.first << "\t" << user.second << "\tData: " << statistics << "\t" << "\tTest name: " << testName;
        )<< "Test case: " << user.first << "\t" << user.second << "\tData: " << statistics << "\t" << "\tTest name: " << testName;
    }  
}

TEST(Controller_getInfoAboutCarWithMaxMileage, TestPositive) {
    Controller controller;
    std::vector<std::tuple<std::pair<std::string, std::string>, std::string, std::string>> table = {
       {{"admin", "admin"}, "ID: 9\n"
                            "Driver info: vadim borisov, id: 31\n"
                            "License: 1111AE-3\n"
                            "Brand: man\n"
                            "Mileage on purchase: 400.00\n"
                            "Load capacity: 6400.00\n"
                            "Summary cost of completed orders: 590.00\n"
                            "Summary load: 10200.00\n"
                            "Summary mileage: 2850.00\n", 
                                                                         "Admin login"},
        {{"disp234", "disp234"},"ID: 9\n"
                                "Driver info: vadim borisov, id: 31\n"
                                "License: 1111AE-3\n"
                                "Brand: man\n"
                                "Mileage on purchase: 400.00\n"
                                "Load capacity: 6400.00\n"
                                "Summary cost of completed orders: 590.00\n"
                                "Summary load: 10200.00\n"
                                "Summary mileage: 2850.00\n", 
                                                                         "Dispatcher login"}
        
    };
    for (const auto& [user, statistics, testName]: table) {
        EXPECT_NO_THROW(
            controller.login(user.first, user.second);
            std::string controllerStatistics = controller.getInfoAboutCarWithMaxMileage();
            EXPECT_EQ(statistics, controllerStatistics)<< "Test case: " << user.first << "\t" << user.second << "\tData: " << statistics << "\t" << "\tTest name: " << testName;
        )<< "Test case: " << user.first << "\t" << user.second << "\tData: " << statistics << "\t" << "\tTest name: " << testName;
    }  
}

TEST(Controller_getInfoAboutCarWithMaxMileage, TestNegative) {
        Controller controller;
    std::vector<std::tuple<std::pair<std::string, std::string>, std::string, std::string>> table = {
       {{"admin", "admin"}, "ID: 10\n"
                            "Driver info: vadim borisov, id: 31\n"
                            "License: 1111AE-3\n"
                            "Brand: man\n"
                            "Mileage on purchase: 400.00\n"
                            "Load capacity: 6400.00\n"
                            "Summary cost of completed orders: 590.00\n"
                            "Summary load: 10200.00\n"
                            "Summary mileage: 2850.00\n", 
                                                                         "Admin login(wrong data)"},
        {{"driver123", "driver123"}, "ID: 9\n"
                            "Driver info: vadim borisov, id: 31\n"
                            "License: 1111AE-3\n"
                            "Brand: man\n"
                            "Mileage on purchase: 400.00\n"
                            "Load capacity: 6400.00\n"
                            "Summary cost of completed orders: 590.00\n"
                            "Summary load: 10200.00\n"
                            "Summary mileage: 2850.00\n", 
                                                                         "Driver login (permission denied)"}
        
    };
    for (const auto& [user, statistics, testName]: table) {
        EXPECT_ANY_THROW(
            try {
                controller.login(user.first, user.second);
                std::string controllerStatistics = controller.getWorstDriverStatistics();
                if(statistics != controllerStatistics) {
                    throw std::invalid_argument("Data is wrong");
                }
            }
            catch(PermissionDeniedException& e) {
                throw;
            }
            catch(std::invalid_argument& e) {
                throw;
            }
        )<< "Test case: " << user.first << "\t" << user.second << "\tData: " << statistics << "\t" << "\tTest name: " << testName;
    }  
}

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



