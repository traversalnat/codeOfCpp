#include <gmock/gmock-matchers.h>
#include <gtest/gtest-death-test.h>
#include <gtest/gtest-param-test.h>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>

#include <iostream>
#include <string>
#include <vector>

using testing::Test;

class Introsort : public Test {};

template <typename T>
T median(T a, T b, T c) {
    if (a < b) {
        if (b < c) {
            return b;
        } else {
            if (a < c) {
                return c;
            } else {
                return a;
            }
        }
    } else {
        if (a < c) {
            return a;
        } else {
            if (b < c) {
                return c;
            } else {
                return b;
            }
        }
    }
}

template <typename RandomAccessIterator, typename T>
RandomAccessIterator unguarded_partition(RandomAccessIterator first,
                                         RandomAccessIterator last, T pivot) {

    while (true) {
        while (*first < pivot)  first++;
        while (*last > pivot)   last--;
        if (last <= first) break;
        std::iter_swap(first, last);
        first++;
    }
    return first;
}

template <typename RandomAccessIterator>
void intro_find(RandomAccessIterator first, RandomAccessIterator last,
                int position) {
    // 保证 position 所在位置的数据与排序完成后对应位置数据一致
    while (last - first > 3) {
        RandomAccessIterator cut = unguarded_partition(
            first, last,
            median((*first), *(first + (last - first) / 2), *(last - 1)));
        if (cut <= first + position) {
            first = cut;
        } else {
            last = cut;
        }
    }

    std::sort(first, last);
}

TEST_F(Introsort, introfind_test) {
    int k = 4;
    std::vector<int> data{1, 3, 9, 3, 8, 11};
    int position = data.size() - k;  // 第k大所在位置
    intro_find(data.begin(), data.end(), position);
    int q = data[position];
    std::sort(data.begin(), data.end());
    ASSERT_EQ(q, data[position]);
}

TEST_F(Introsort, large_random_introfind_test) {
    int k = 7;
    std::vector<int> data(10);
    std::generate(data.begin(), data.end(), []() { return std::rand() % 100; });
    int position = data.size() - k;  // 第k大所在位置
    intro_find(data.begin(), data.end(), position);
    int q = data[position];
    std::sort(data.begin(), data.end());
    ASSERT_EQ(q, data[position]);
}
