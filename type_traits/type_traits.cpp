#include <gmock/gmock-matchers.h>
#include <gtest/gtest-death-test.h>
#include <gtest/gtest-param-test.h>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>

#include <iostream>
#include <string>
#include <vector>

using testing::Test;
class Type_Traits : public Test {
};

struct __false_type {};
struct __true_type {};

template <typename type>
struct type_traits {
    typedef __false_type has_prefix_increment;
    typedef __false_type has_post_increment;
};

struct Data {
public:
    Data(int i) : i {i} {}
    // prefix increment
    Data& operator++() {
        i++;
        return *this;
    }

    // post increment, 参数必须是 int
    Data operator++(int) {
        // 比较难实现先返回后自增(返回自增前的值)
        i++;
        return *this;
    }

private:
    int i;
};


// 设置用户自定义类型的type_traits
template <>
struct type_traits<Data> {
    typedef __false_type has_post_increment;
};

template <typename T>
void advance(T t) {
    return __advance(t, type_traits<Data>::has_prefix_increment());
}

template <typename T>
void __advance(T t, __true_type) {
    ++t;
    std::cout << "prefix self increment\n";
}

template <typename T>
void __advance(T t, __false_type) {
    t++;
    std::cout << "post self increment\n";
}


TEST_F(Type_Traits, type_traits_test) {
    Data d {4};
    advance(d);
}
