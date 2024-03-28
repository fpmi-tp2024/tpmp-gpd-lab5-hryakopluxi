//
// Created by Stanislau Senkevich on 21.3.24.
//

#include "../include/view.h"

std::ostream& operator<<(std::ostream &os, const Order& o) {
    os << "Order ID: " << o.getId() << "\n";
    os << "Driver ID: " << o.getDriverId() << "\n";
    os << "Car ID: " << o.getCarId() << "\n";
    os << "Date: " << o.getDate() << "\n";
    os << "Mileage: " << o.getMileage() << "\n";
    os << "Load: " << o.getLoad() << "\n";
    os << "Cost: " << o.getCost() << "\n";
    if (o.getIsApproved()) {
        os << "Approved: Yes\n";
    } else {
        os << "Approved: No\n";
    }
    return os;
}

View::View() {
    controller = Controller(db_filename);
}

View::View(const std::string& login, const std::string& password) {
    controller = Controller(db_filename);
    controller.login(login, password);
}

void View::login() {
    std::string login, password;
    std::cout << "Enter login: ";
    std::getline(std::cin, login, '\n');
    std::cout << "Enter password: ";
    std::getline(std::cin, password, '\n');

    bool res;

    try {
        res = controller.login(login, password);
    } catch (const std::exception& e) {
        std::cout << e.what() << "\n";
        return;
    }

    if (!res) {
        std::cout << "Invalid login or password\n";
        return;
    }

    std::cout << "Logged in successfully!\nYour ID: " << controller.getUserId() << "\n";
}

void View::logout() {
    controller.logout();
}

void View::getDriverOrders() const {
    int id;
    std::string start, end;
    std::cout << "Enter Driver ID: ";
    std::cin >> id;
    std::cout << "Enter start of the period (YYYY-MM-DD): ";
    std::cin >> start;
    std::cout << "Enter end of the period (YYYY-MM-DD): ";
    std::cin >> end;

    std::vector <Order> orders = controller.getDriverOrders(id, start, end);

    if (orders.empty()) {
        std::cout << "\nNo orders found during this period\n";
        return;
    }

    for (auto& o : orders) {
        std::cout << "---------------------------------\n";
        std::cout << o;
    }
    std::cout << "---------------------------------\n";
}

void View::getCarSummaryMileageAndLoads() const {
    int id;
    std::cout << "Enter Car ID: ";
    std::cin >> id;
    std::string res;
    try {
        res = controller.getCarSummaryMileageAndLoad(id);
    } catch (const std::exception& e) {
        std:: cout << e.what() << "\n";
        return;
    }
    std::cout << "---------------------------------\n";
    std::cout << res;
    std::cout << "---------------------------------\n";
}

void View::getDriverStatistics() const {
    int id;
    std::cout << "Enter Driver ID: ";
    std::cin >> id;
    std::string res;
    try {
        res = controller.getDriverStatistics(id);
    } catch (const std::exception& e) {
        std::cout << e.what() << "\n";
        return;
    }

    std::cout << "---------------------------------\n";
    std::cout << res;
    std::cout << "---------------------------------\n";
}

void View::getWorstDriverStatistics() const {
    std::string res;
    try {
        res = controller.getWorstDriverStatistics();
    } catch (const std::exception& e) {
        std::cout << e.what() << "\n";
        return;
    }

    std::cout << "---------------------------------\n";
    std::cout << res;
    std::cout << "---------------------------------\n";
}

void View::getInfoAboutCarWithMaxMileage() const {
    std::string res;
    try {
        res = controller.getInfoAboutCarWithMaxMileage();
    } catch (const std::exception& e) {
        std::cout << e.what() << "\n";
        return;
    }

    std::cout << "---------------------------------\n";
    std::cout << res;
    std::cout << "---------------------------------\n";
}

void View::storeDriversEarnedMoney() {
    std::string start, end;
    std::cout << "Enter start of the period (YYYY-MM-DD): ";
    std::cin >> start;
    std::cout << "Enter end of the period (YYYY-MM-DD): ";
    std::cin >> end;

    std::vector<std::string> res;

    try {
        res = controller.storeDriversEarnedMoney(start, end);
    } catch (const std::exception& e) {
        std::cout << e.what() << "\n";
        return;
    }

    for (auto& s : res) {
        std::cout << "---------------------------------\n";
        std::cout << s;
    }
    std::cout << "---------------------------------\n";

}

