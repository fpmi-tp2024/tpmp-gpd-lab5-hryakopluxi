//
// Created by Stanislau Senkevich on 21.3.24.
//

#include "../include/view.h"

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