//
// Created by Stanislau Senkevich on 30.3.24.
//

#include <gtest/gtest.h>
#include "../include/controller.h"
#include <sstream>

TEST(TestConnection, TestPositive) {
    EXPECT_NO_THROW(Controller controller("db/app.db"));
}

TEST(TestConnection, TestNegative) {
    EXPECT_ANY_THROW(Controller controller("db/notapp.db"));
}

