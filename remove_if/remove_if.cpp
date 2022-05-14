#include <gmock/gmock-matchers.h>
#include <gtest/gtest-death-test.h>
#include <gtest/gtest-param-test.h>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>

#include <iostream>
#include <string>
#include <vector>

using testing::Test;
class Remove_If : public Test {};

template <typename ForwardIterator, typename Predicate>
ForwardIterator __remove_if(ForwardIterator first, ForwardIterator last,
                          Predicate pre) {
    // 将不满足 pre 条件的元素移至 iterator 头部, 最后 [first, last) 是满足 pre 条件的元素
    first = std::find_if(first, last, pre);
    if (first != last) {
        ForwardIterator __i = first;
        while (__i++ != last) {
            if (!(pre(*__i))) {
                *first = std::move(*__i);
                first++;
            }
        }
    }
    return first;
}

TEST_F(Remove_If, remove_if_test) {
    int core[9] = {4, 3, 12, 2, 5, 4, 3, 1, 0};
    std::vector<int> ver{3, 2, 4, 1, 2, 4, 5, 6, 7};

    for (auto k : ver) {
        std::cout << k << " ";
    }
    std::cout << "\n";
    ver.erase(__remove_if(ver.begin(), ver.end(), [&](int a) { return a < 4; }),
              ver.end());
    // __remove_if(ver.begin(), ver.end(), [&](int a) { return a < 4; });

    for (auto k : ver) {
        std::cout << k << " ";
    }
    std::cout << "\n";
}
