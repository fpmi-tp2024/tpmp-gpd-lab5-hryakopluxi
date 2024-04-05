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

    std::cout << "Logged in successfully!\n";
}

void View::logout() {
    controller.logout();
}

void View::changeDb() {
    std::string path;
    std::cout << "Enter path to the db: ";
    std::getline(std::cin, path, '\n');
    Controller newController;
    try {
        newController = Controller(path);
    } catch (const std::exception& e) {
        std::cout << "Cannot open database\n";
        return;
    }
    controller = newController;
    std::cout << "Database successfully changed\n";
}

void View::getDriverOrders(int driver_id) const {
    std::string start, end;
    std::cout << "Enter start of the period (YYYY-MM-DD): ";
    std::getline(std::cin, start, '\n');
    std::cout << "Enter end of the period (YYYY-MM-DD): ";
    std::getline(std::cin, end, '\n');
    std::vector <Order> orders;

    try {
        orders = controller.getDriverOrders(driver_id, start, end);
    } catch (const std::exception& e) {
        std::cout << e.what() << "\n";
        return;
    }

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
    id = inputInt();
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

void View::getDriverStatistics(int driver_id) const {
    std::string res;
    try {
        res = controller.getDriverStatistics(driver_id);
    } catch (const std::exception& e) {
        std::cout << e.what() << "\n";
        return;
    }

    std::cout << "---------------------------------\n";
    std::cout << res;
    std::cout << "---------------------------------\n";
}

void View::getAllDriverStatistics() const {
    std::vector<std::string> res;
    try {
        res = controller.getAllDriversStatistics();
    } catch (const std::exception& e) {
        std::cout << e.what() << "\n";
        return;
    }

    if (res.empty()) {
        std::cout << "No driver was found.\n";
        return;
    }

    for (auto& s : res) {
        std::cout << "---------------------------------\n";
        std::cout << s;
    }
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
    std::getline(std::cin, start, '\n');
    std::cout << "Enter end of the period (YYYY-MM-DD): ";
    std::getline(std::cin, end, '\n');

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

void View::getDriverEarnedMoney(int driver_id) const {
    std::string start, end;
    double res;
    std::cout << "Enter start of the period (YYYY-MM-DD): ";
    std::getline(std::cin, start, '\n');
    std::cout << "Enter end of the period (YYYY-MM-DD): ";
    std::getline(std::cin, end, '\n');

    try {
        res = controller.getDriverEarnedMoney(driver_id, start, end);
    } catch (const std::exception& e) {
        std::cout << e.what() << "\n";
        return;
    }

    char buf[25];
    sprintf(buf, "Earned money: %.2f\n", res);

    std::cout << buf;
}

void View::addCar() {
    int id;
    Car car;
    try {
        car.getDataFromConsole();
    } catch (const std::exception& e) {
        std::cout << "Invalid input data\n";
        return;
    }

    try {
        id = controller.addCar(car);
    } catch (const std::exception& e) {
        std::cout << e.what() << "\n";
        return;
    }

    std::cout << "Car successfully added. New ID: " << id << "\n";
}

void View::addDriver() {
    int id;
    Driver driver;
    try {
        driver.getDataFromConsole();
    } catch (const std::exception& e) {
        std::cout << "Invalid input data\n";
        return;
    }

    try {
        id = controller.addDriver(driver);
    } catch (const std::exception& e) {
        std::cout << e.what() << "\n";
        return;
    }
    std::cout << "Driver successfully added. New ID: " << id << "\n";
}

void View::addOrder() {
    int id;
    Order order;
    try {
        order.getDataFromConsole();
    } catch (const std::exception& e) {
        std::cout << "Invalid input data\n";
        return;
    }

    try {
        id = controller.addOrder(order);
    } catch (const std::exception& e) {
        std::cout << e.what() << "\n";
        return;
    }
    std::cout << "Order successfully added. New ID: " << id << "\n";
}

void View::addDispatcher() {
    int id;
    Dispatcher dispatcher;
    try {
        dispatcher.getDataFromConsole();
    } catch (const std::exception& e) {
        std::cout << "Invalid input data\n";
        return;
    }

    try {
        id = controller.addDispatcher(dispatcher);
    } catch (const std::exception& e) {
        std::cout << e.what() << "\n";
        return;
    }
    std::cout << "Dispatcher successfully added. New ID: " << id << "\n";
}

void View::updateCar() {
    int id;
    Car update;
    std::cout << "Enter Car ID to update: ";
    id = inputInt();
    try {
        update.getDataFromConsole();
    } catch (const std::exception& e) {
        std::cout << "Invalid input data\n";
        return;
    }

    try {
        controller.updateCar(id, update);
    } catch (const std::exception& e) {
        std::cout << e.what() << "\n";
        return;
    }

    std::cout << "Car successfully updated.\n";
}

void View::updateDriver() {
    int id;
    Driver update;
    std::cout << "Enter Driver ID to update: ";
    id = inputInt();
    try {
        update.getDataFromConsole();
    } catch (const std::exception& e) {
        std::cout << "Invalid input data\n";
        return;
    }

    try {
        controller.updateDriver(id, update);
    } catch (const std::exception& e) {
        std::cout << e.what() << "\n";
        return;
    }

    std::cout << "Driver successfully updated.\n";
}

void View::updateOrder() {
    int id;
    Order update;
    std::cout << "Enter Order ID to update: ";
    id = inputInt();
    try {
        update.getDataFromConsole();
    } catch (const std::exception& e) {
        std::cout << "Invalid input data\n";
        return;
    }

    try {
        controller.updateOrder(id, update);
    } catch (const std::exception& e) {
        std::cout << e.what() << "\n";
        return;
    }

    std::cout << "Order successfully updated.\n";
}

void View::updateOrderApproveStatus() {
    int id, st;
    st = -1;
    std::cout << "Enter Order ID to update status: ";
    id = inputInt();
    while (st != 0 && st != 1) {
        std::cout << "Enter new status (1 - approved, 0 - disapproved): ";
        st = inputInt();
    }

    try {
        controller.updateOrderApproveStatus(id, st);
    } catch (const std::exception& e) {
        std::cout << e.what() << "\n";
        return;
    }
}

void View::updateDispatcher() {
    int id;
    Dispatcher update;
    std::cout << "Enter Dispatcher ID to update: ";
    id = inputInt();
    try {
        update.getDataFromConsole();
    } catch (const std::exception& e) {
        std::cout << "Invalid input data\n";
        return;
    }

    try {
        controller.updateDispatcher(id, update);
    } catch (const std::exception& e) {
        std::cout << e.what() << "\n";
        return;
    }

    std::cout << "Dispatcher successfully updated.\n";
}

void View::updateUser(int user_id) {
    User update;
    try {
        update.getDataFromConsole();
    } catch (const std::exception& e) {
        std::cout << "Invalid input data\n";
        return;
    }
    try {
        controller.updateUser(user_id, update);
    } catch (const std::exception& e) {
        std::cout << e.what() << "\n";
        return;
    }

    std::cout << "User successfully updated.\n";
}

void View::deleteCar() {
    int id;
    std::cout << "Enter Car ID to delete: ";
    id = inputInt();

    try {
        controller.deleteCar(id);
    } catch (const std::exception& e) {
        std::cout << e.what() << "\n";
        return;
    }

    std::cout << "Car successfully deleted.\n";
}

void View::deleteDriver() {
    int id;
    std::cout << "Enter Driver ID to delete: ";
    id = inputInt();

    try {
        controller.deleteDriver(id);
    } catch (const std::exception& e) {
        std::cout << e.what() << "\n";
        return;
    }

    std::cout << "Driver successfully deleted.\n";
}

void View::deleteDispatcher() {
    int id;
    std::cout << "Enter Dispatcher ID to delete: ";
    id = inputInt();

    try {
        controller.deleteDispatcher(id);
    } catch (const std::exception& e) {
        std::cout << e.what() << "\n";
        return;
    }

    std::cout << "Dispatcher successfully deleted.\n";
}

void View::deleteOrder() {
    int id;
    std::cout << "Enter Order ID to delete: ";
    id = inputInt();

    try {
        controller.deleteOrder(id);
    } catch (const std::exception& e) {
        std::cout << e.what() << "\n";
        return;
    }

    std::cout << "Order successfully deleted.\n";
}

