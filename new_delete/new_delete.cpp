#include <gmock/gmock-matchers.h>
#include <gtest/gtest-death-test.h>
#include <gtest/gtest-param-test.h>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>

#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

using testing::Test;
class New_Delete : public Test {
};

class A {
public:
    A(int a) : a {a} {}
    ~A() {
        std::cout << "free " << a << std::endl;
    }

public:
    int a;
};

TEST_F(New_Delete, new_delete_test) {
    A *arr = new A[4] {1, 2, 3, 4};
    // 1. 得到 arr 前的数组长度
    size_t *size = (size_t *)((char *) arr - sizeof(size_t));
    // 2. 调用数组中每一个结构体析构函数
    for (int i = 0; i < *size; i++) {
        arr[i].~A();
    }
    // 3. 释放整块内存
    free(size);
}

