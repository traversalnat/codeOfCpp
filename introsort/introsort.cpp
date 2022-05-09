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

template <typename Size>
Size _lg(Size n) {
    Size k;
    for (k = 0; n > 1; n >>= 1) ++k;
    return k;
}

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
        while (*first < pivot) ++first;
        --last;
        while (*last > pivot) --last;
        if (!(first < last)) return first;
        std::iter_swap(first, last);
        ++first;
    }
}

template <typename RandomAccessIterator, typename Size>
void intro_sort_loop(RandomAccessIterator first, RandomAccessIterator last,
                     Size depth_limit) {
    using T = typename RandomAccessIterator::value_type;
    while (last - first > 16) {
        if (depth_limit == 0) {
            // heap sort
            std::partial_sort(first, last, last);
            return;
        }

        --depth_limit;

        RandomAccessIterator cut = unguarded_partition(
            first, last,
            T(median(*first, *(first + (last - first) / 2), *last)));

        intro_sort_loop(cut, last, depth_limit);
        intro_sort_loop(first, cut, depth_limit);
    }
}

template <typename RandomAccessIterator>
void __linear_insert(RandomAccessIterator first, RandomAccessIterator last) {
    using T = typename RandomAccessIterator::value_type;
    T value = *last;
    if (value < *first) {
        // 将 *last 插入到头部，其余后退一位
        std::copy_backward(first, last, last + 1);
        *first = value;
    } else {
        RandomAccessIterator next = last;
        --next;
        while (value < *next) {
            *last = *next;
            last = next;
            --next;
        }
        *last = value;
    }
}

template <typename RandomAccessIterator>
void insertion_sort(RandomAccessIterator first, RandomAccessIterator last) {
    if (first == last) return;
    for (RandomAccessIterator i = first + 1; i != last; i++) {
        __linear_insert(first, i);
    }
}

template <typename RandomAccessIterator>
void intro_sort(RandomAccessIterator first, RandomAccessIterator last) {
    if (first != last) {
        // while last - first > 16
        intro_sort_loop(first, last, _lg(last - first) * 2);
        insertion_sort(first, last);
    }
}

TEST_F(Introsort, median) {
    ASSERT_EQ(6, median(6, 2, 9));
    ASSERT_EQ(6, median(2, 6, 9));
    ASSERT_EQ(6, median(9, 2, 6));
}

TEST_F(Introsort, introsort_test) {
    std::vector<int> data{6, 5, 4, 7, 2, 3, 1};
    std::vector<int> ans{1, 2, 3, 4, 5, 6, 7};
    intro_sort(data.begin(), data.end());
    for (int i : data) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    ASSERT_EQ(data, ans);
}

TEST_F(Introsort, large_random_introsort_test) {
    std::vector<int> data{100};
    std::generate(data.begin(), data.end(), []() { return std::rand(); });
    intro_sort(data.begin(), data.end());
    ASSERT_TRUE(std::is_sorted(data.begin(), data.end()));
}
