#include <gmock/gmock-matchers.h>
#include <gtest/gtest-death-test.h>
#include <gtest/gtest-param-test.h>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>

#include <iostream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

using testing::Test;
class Invoke : public Test {};

template <class _Tp>
inline _Tp&& my_forward(typename std::remove_reference<_Tp>::type& t) {
    return static_cast<_Tp&&>(t);
}

template <class _Tp>
inline _Tp&& my_forward(typename std::remove_reference<_Tp>::type&& t) {
    static_assert(!std::is_lvalue_reference<_Tp>::value,
                  "can not forward an rvalue as an lvalue");
    return static_cast<_Tp&&>(t);
}

/*
    template <class T>
    void foo() noexcept(noexcept(T())) {}
    表示 foo() 是否抛出异常取决于 T() 是否抛出异常
    1. noexcept(expression) 表示 expression 是否抛出异常
    2. noexcept(false/true) 用在函数声明后面表示函数是否抛出异常
    3. foo() noexcept(true) 等同于 foo() noexcept
 */
#define INVOKE_RETURN(...)                                   \
    noexcept(noexcept(__VA_ARGS__))->decltype(__VA_ARGS__) { \
        return __VA_ARGS__;                                  \
    }

/**
 * @brief my_invoke(f, args...)
 *
 * @tparam _Fp
 * @tparam _Args
 * @param __f   函数指针
 * @param __args 函数参数
 *
 * @return decltype(std::forward<_Fp>(__f)(std::forward<_Args>(__args)...))
 */
template <class _Fp, class... _Args>
inline auto my_invoke(_Fp&& __f, _Args&&... __args)
    INVOKE_RETURN(my_forward<_Fp>(__f)(std::forward<_Args>(__args)...));

void func(int a, int b) { std::cout << a + b << std::endl; }

TEST_F(Invoke, invoke_test) { 
    my_invoke(std::cref(func), 1, 2); 
}
