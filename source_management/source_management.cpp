#include <gmock/gmock-matchers.h>
#include <gtest/gtest-death-test.h>
#include <gtest/gtest-param-test.h>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>

#include <iostream>
#include <memory>
#include <string>
#include <vector>

using testing::Test;
class Source_Management : public Test {};

// 模拟 rust
class Vector {
 public:
    explicit Vector(int sz);
    // 不允许浅拷贝
    Vector(const Vector &x) = delete;
    Vector &operator=(const Vector &x) = delete;
    // 所有权移动
    Vector(Vector &&x);
    Vector &operator=(Vector &&x);
    bool operator==(const Vector &x) const;
    // 深拷贝
    Vector clone();

    double &operator[](int i);

    int size() const { return sz; }

    ~Vector();

 private:
    int sz;
    double *elem;
};

Vector::Vector(int sz) {
    if (sz < 0) {
        throw std::length_error("Vector sz should > 0");
    }
    elem = new double[sz];
    this->sz = sz;
}

Vector::Vector(Vector &&x) : sz{x.sz}, elem{x.elem} {
    x.elem = nullptr;
    x.sz = 0;
}

Vector::~Vector() { delete[] elem; }

Vector &Vector::operator=(Vector &&x) {
    elem = x.elem;
    sz = x.sz;
    x.elem = nullptr;
    x.sz = 0;
    return *this;
}

double &Vector::operator[](int i) { return elem[i]; }

bool Vector::operator==(const Vector &x) const {
    if (sz != x.sz) {
        return false;
    }
    for (int i = 0; i < sz; i++) {
        if (elem[i] != x.elem[i]) {
            return false;
        }
    }
    return true;
}

Vector Vector::clone() {
    Vector v{this->sz};
    for (int i = 0; i < this->sz; i++) {
        v[i] = elem[i];
    }
    return v;
}

bool is_empty(Vector &v) {
    return v.size() == 0;
}

template <typename T>
using Arc = std::shared_ptr<T>;

TEST_F(Source_Management, source_manage_like_rust) {
    // 所有权移动，使用移动构造函数模拟，禁用拷贝构造、赋值函数
    auto v1 = Vector{3};
    auto v2 = std::move(v1);
    // 所有权表示旧的资源句柄被复制到新的资源句柄并且被清空
    ASSERT_EQ(0, v1.size());    

    // 使用资源句柄作为参数，必须使用引用（拷贝构造函数被禁用）
    ASSERT_TRUE(!is_empty(v2));    

    // 使用引用来访问资源句柄的数据（读写资源的成员函数返回引用）
    // 相当于 rust &mut, 但是引用的使用是隐式的
    v2[1] = 3;
    ASSERT_EQ(3, v2[1]);

    // 使用智能指针使得资源句柄可以存在多个备份
    Arc<Vector> v3 { new Vector{3} };
    std::vector<Arc<Vector>> container1;
    std::vector<Arc<Vector>> container2;
    container1.push_back(v3);
    container2.push_back(v3);

    for (auto x : container1) {
        std::cout << x->size() << std::endl;
    }

    for (auto x : container2) {
        std::cout << x->size() << std::endl;
    }
}
