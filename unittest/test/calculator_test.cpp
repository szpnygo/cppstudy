#include "calculator.h"

#include <gtest/gtest.h>

// 定义一个测试夹具
class CalculatorTest : public ::testing::Test {
  protected:
    Calculator calculator; // Calculator 对象

    // 在每个测试用例运行之前，SetUp 会被调用
    void SetUp() override {
        // 可以在此设置你的测试场景，如果需要的话
    }

    // 在每个测试用例运行结束后，TearDown 会被调用
    void TearDown() override {
        // 可以在此清理你的测试场景，如果需要的话
    }
};

// 使用测试夹具 TEST_F 代替 TEST
TEST_F(CalculatorTest, AddTest) {
    EXPECT_EQ(calculator.add(2, 2), 4);
    EXPECT_EQ(calculator.add(-2, 2), 0);
}

TEST_F(CalculatorTest, SubtractTest) {
    EXPECT_EQ(calculator.subtract(2, 2), 0);
    EXPECT_EQ(calculator.subtract(2, -2), 4);
}

TEST_F(CalculatorTest, MultiplyTest) {
    EXPECT_EQ(calculator.multiply(2, 2), 4);
    EXPECT_EQ(calculator.multiply(-2, 2), -4);
    EXPECT_EQ(calculator.multiply(2, 0), 0);
}

TEST_F(CalculatorTest, DivideTest) {
    EXPECT_DOUBLE_EQ(calculator.divide(4, 2), 2.0);
    EXPECT_THROW(calculator.divide(4, 0), std::invalid_argument);
}
