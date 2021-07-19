#include <gmock/gmock-matchers.h>
#include <gtest/gtest-death-test.h>
#include <gtest/gtest-param-test.h>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>

#include <functional>
#include <iostream>
#include <string>
#include <vector>

using testing::Test;
class Function : public Test {};

template <typename T, typename Iter, typename Collect>
T sum_if(const T res, const Iter& iter, Collect func) {
    T result = res;
    for (auto x : iter) {
        if (func(x)) {
            result += x;
        }
    }
    return result;
}

template <typename T>
void println(std::vector<T> iter) {
    for (auto x : iter) {
        std::cout << x << " ";
    }
    std::cout << "\n";
}

TEST_F(Function, function_test) {
    using std::cout;
    using std::vector;
    vector<int> iter{1, 2, 3, 4};
    int sum = sum_if<int>(0, iter, [](int a) { return true; });
    int iter_sum = 0;
    for (auto x : iter) {
        iter_sum += x;
    }
    ASSERT_EQ(iter_sum, sum);
}

TEST_F(Function, template_test) {
    using std::vector;
    typedef double elem_type;

    vector<elem_type> iter{0.1, 0.2, 0.3, 0.4};
    elem_type sum =
        sum_if<elem_type>(0, iter, [](elem_type a) { return true; });
    ASSERT_EQ(1.0, sum);
}
