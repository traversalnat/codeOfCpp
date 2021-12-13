#include <gmock/gmock-matchers.h>
#include <gtest/gtest-death-test.h>
#include <gtest/gtest-param-test.h>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>

#include <iostream>
#include <string>
#include <vector>
#include "any.h"

using testing::Test;
using hlp::any;
using hlp::any_cast;

class Any_Object : public Test {
};

TEST_F(Any_Object, hlp_any_test) {
    // 测试引用
    int i = 3;
    int& ii = i;
    any any_ii = ii;
    ASSERT_EQ(i, any_cast<int>(any_ii));

    // 测试指针
    size_t *t = new size_t {3};
    any any_t = t; // 危险, any 持有的是 t 的复制, t 被释放后, any_t 将访问nullptr
    ASSERT_EQ(*t, *any_cast<size_t*>(any_t));
}
