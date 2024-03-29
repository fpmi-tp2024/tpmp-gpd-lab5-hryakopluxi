//
// Created by hakeyn on 29.3.24.
//
#include "../include/view.h"

const std::string View::GUEST_MENU_MSG = "Menu:\n"
                                         "(0) Exit\n"
                                         "(1) Login\n";

const std::string View::DRIVER_MENU_MSG = "Menu:\n"
                                          "(1) Get orders\n"
                                          "(2) Get car's summary mileage and load\n"
                                          "(3) Get statistics\n"
                                          "(4) Get earned money\n"
                                          "(5) Add order for approval\n"
                                          "(6) Update login or password\n"
                                          "(7) Logout\n"
                                          "(0) Exit\n"
                                          "Your ID: ";

const std::string View::DISPATCHER_MENU_MSG = "Menu:\n"
                                              "(1) Get driver's orders\n"
                                              "(2) Get car's summary mileage and load\n"
                                              "(3) Get driver's statistics\n"
                                              "(4) Get all drivers' statistics\n"
                                              "(5) Get worst driver's statistics\n"
                                              "(6) Get info about car with highest mileage\n"
                                              "(7) Get driver's earnings\n"
                                              "(8) Store all drivers' earnings\n"
                                              "(9) Add car\n"
                                              "(10) Add driver\n"
                                              "(11) Add order\n"
                                              "(12) Update car info\n"
                                              "(13) Update driver info\n"
                                              "(14) Update order info\n"
                                              "(15) Update order status\n"
                                              "(16) Update login or password\n"
                                              "(17) Logout\n"
                                              "(0) Exit\n"
                                              "Your ID: ";

const std::string View::ADMIN_MENU_MSG = "Menu:\n"
                                         "(1) Get driver's orders\n"
                                         "(2) Get car's summary mileage and load\n"
                                         "(3) Get driver's statistics\n"
                                         "(4) Get all drivers' statistics\n"
                                         "(5) Get worst driver's statistics\n"
                                         "(6) Get info about car with highest mileage\n"
                                         "(7) Get driver's earnings\n"
                                         "(8) Store all drivers' earnings\n"
                                         "(9) Add car\n"
                                         "(10) Add driver\n"
                                         "(11) Add order\n"
                                         "(12) Add dispatcher\n"
                                         "(13) Update car info\n"
                                         "(14) Update driver info\n"
                                         "(15) Update order info\n"
                                         "(16) Update order status\n"
                                         "(17) Update dispatcher info\n"
                                         "(18) Update login or password\n"
                                         "(19) Delete car\n"
                                         "(20) Delete driver\n"
                                         "(21) Delete order\n"
                                         "(22) Delete dispatcher\n"
                                         "(23) Logout\n"
                                         "(0) Exit\n"
                                         "Your ID: ";
void View::pressAnyKey() const {
    std::cin.ignore();
    std::string msg = "Press Enter to continue...\n";
    std::cout << msg;
    std::getline(std::cin, msg);
}

bool View::guestMenu() {
    int opt = -1;
    std::cout << GUEST_MENU_MSG;
    std::cout << "Command: ";
    std::cin >> opt;
    switch (opt) {
        case 0:
            return true;
        case 1:
            login();
            return false;
        case 2:

        default:
            std::cout << "Unknown command\n";
    }
}

bool View::driverMenu() {
    int opt;
    int user_id = controller.getUserId();
    std::cout << DRIVER_MENU_MSG << controller.getUserId() << "\n";
    std::cout << "Command: ";
    std::cin >> opt;
    switch (opt) {
        case 0:
            return true;
        case 1:
            getDriverOrders(user_id);
            break;
        case 2:
            getCarSummaryMileageAndLoads();
            break;
        case 3:
            getDriverStatistics(user_id);
            break;
        case 4:
            getDriverEarnedMoney(user_id);
            break;
        case 5:
            addOrder();
            break;
        case 6:
            updateUser(user_id);
            break;
        case 7:
            logout();
            break;
        default:
            std::cout << "Unknown command\n";
    }
    pressAnyKey();
    return false;
}

bool View::dispatcherMenu() {
    int opt, id;
    opt = -1;
    int user_id = controller.getUserId();
    std::cout << DISPATCHER_MENU_MSG << controller.getUserId() << "\n";
    std::cout << "Command: ";
    std::cin >> opt;
    switch (opt) {
        case 0:
            return true;
        case 1:
            std::cout << "Enter driver's ID: ";
            std::cin >> id;
            getDriverOrders(id);
            break;
        case 2:
            getCarSummaryMileageAndLoads();
            break;
        case 3:
            std::cout << "Enter driver's ID: ";
            std::cin >> id;
            getDriverStatistics(id);
            break;
        case 4:
            getAllDriverStatistics();
            break;
        case 5:
            getWorstDriverStatistics();
            break;
        case 6:
            getInfoAboutCarWithMaxMileage();
            break;
        case 7:
            std::cout << "Enter driver's ID: ";
            std::cin >> id;
            getDriverEarnedMoney(id);
            break;
        case 8:
            storeDriversEarnedMoney();
            break;
        case 9:
            addCar();
            break;
        case 10:
            addDriver();
            break;
        case 11:
            addOrder();
            break;
        case 12:
            updateCar();
            break;
        case 13:
            updateDriver();
            break;
        case 14:
            updateOrder();
            break;
        case 15:
            updateOrderApproveStatus();
            break;
        case 16:
            updateUser(user_id);
            break;
        case 17:
            logout();
            break;
        default:
            std::cout << "Unknown command\n";
    }
    pressAnyKey();
    return false;
}

bool View::adminMenu() {
    int opt, id;
    opt = -1;
    int user_id = controller.getUserId();
    std::cout << ADMIN_MENU_MSG << controller.getUserId() << "\n";
    std::cout << "Command: ";
    std::cin >> opt;
    switch (opt) {
        case 0:
            return true;
        case 1:
            std::cout << "Enter driver's ID: ";
            std::cin >> id;
            getDriverOrders(id);
            break;
        case 2:
            getCarSummaryMileageAndLoads();
            break;
        case 3:
            std::cout << "Enter driver's ID: ";
            std::cin >> id;
            getDriverStatistics(id);
            break;
        case 4:
            getAllDriverStatistics();
            break;
        case 5:
            getWorstDriverStatistics();
            break;
        case 6:
            getInfoAboutCarWithMaxMileage();
            break;
        case 7:
            std::cout << "Enter driver's ID: ";
            std::cin >> id;
            getDriverEarnedMoney(id);
            break;
        case 8:
            storeDriversEarnedMoney();
            break;
        case 9:
            addCar();
            break;
        case 10:
            addDriver();
            break;
        case 11:
            addOrder();
            break;
        case 12:
            addDispatcher();
            break;
        case 13:
            updateCar();
            break;
        case 14:
            updateDriver();
            break;
        case 15:
            updateOrder();
            break;
        case 16:
            updateOrderApproveStatus();
            break;
        case 17:
            updateDispatcher();
        case 18:
            updateUser(user_id);
            break;
        case 19:
            deleteCar();
            break;
        case 20:
            deleteDriver();
            break;
        case 21:
            deleteOrder();
            break;
        case 22:
            deleteDispatcher();
            break;
        case 23:
            logout();
            break;
        default:
            std::cout << "Unknown command\n";
    }
    pressAnyKey();
    return false;
}

void View::menu() {
    while (true) {
        switch (controller.getUserRole()) {
            case GUEST:
                if (guestMenu()) {
                    return;
                }
                break;
            case DRIVER:
                if (driverMenu()) {
                    return;
                }
                break;
            case DISPATCHER:
                if (dispatcherMenu()) {
                    return;
                }
                break;
            case ADMIN:
                if (adminMenu()) {
                    return;
                }
                break;
            default:
                return;
        }
    }

}
