#include <gmock/gmock-matchers.h>
#include <gtest/gtest-death-test.h>
#include <gtest/gtest-param-test.h>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <numeric>

using testing::Test;
using namespace std;
class Functor : public Test {
};

template <class T>
inline
T identity_element(multiplies<T>) {
    return T{1};
}

TEST_F(Functor, functor_test) {
    vector<int> iv {1, 2, 3, 4, 5};
    auto ans = accumulate(iv.begin(), iv.end(), identity_element(multiplies<int>()), multiplies<int>());
    ASSERT_EQ(120, ans);

    unary_negate<negate<int>> not_negate((negate<int>()));

    auto neg = negate<int>();

    ASSERT_EQ(20, neg(-20));
    ASSERT_EQ(false, not_negate(20));
}
