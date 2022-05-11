#include <gmock/gmock-matchers.h>
#include <gtest/gtest-death-test.h>
#include <gtest/gtest-param-test.h>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>

#include <iostream>
#include <string>
#include <vector>

using testing::Test;
class Traits : public Test {};

// tag 的使用
struct java_tag {};
struct c_tag {};
struct cpp_tag : public c_tag {};

// 定义一个通用类型 lang_iter, 定义了该类型所需要包括的一些类型信息
template <typename Lang, typename T, typename Pointer = T*,
          typename Reference = T&>
struct lang_iter {
    // 定义了 lang_iter 需要的 Lang 信息, 可以是 java_tag、c_tag、cpp_tag
    typedef Lang lang_category;
    typedef T value_type;
    typedef Pointer pointer;
    typedef Reference reference;
};

// lang_traits 可以从 lang_iter 通用类型中得到具体类型信息
template <class Lang>
struct lang_traits {
    typedef typename Lang::lang_category lang_category;
    typedef typename Lang::value_type value_type;
    typedef typename Lang::pointer pointer;
    typedef typename Lang::reference reference;
};

// 用户自定义类型
template <typename T>
struct Java_Iter :
    public lang_iter<java_tag, T> 
{
    
};

template <typename T>
struct C_Iter :
    public lang_iter<c_tag, T> 
{
    
};

template <typename T>
struct Cpp_Iter :
    public lang_iter<cpp_tag, T> 
{
    
};

// Traits 的作用是从类型中取得一些信息，从而使得泛化的函数能够针对特定类型调用特定函数
template <typename LangIter>
void iter(LangIter iter) {
    __iter(iter, typename LangIter::lang_category());
    // or
    __iter(iter, typename lang_traits<LangIter>::lang_category());
    std::iterator_traits<class _Iter>;
}

template <typename LangIter>
void __iter(LangIter iter, java_tag) {
    std::cout << "java iter" << std::endl;
}

template <typename LangIter>
void __iter(LangIter iter, c_tag) {
    std::cout << "c iter" << std::endl;
}

template <typename LangIter>
void __iter(LangIter iter, cpp_tag) {
    // 简单调用 c_tag
    __iter(iter, c_tag());
}


TEST_F(Traits, traits_test) {
    Java_Iter<int> java_iter;
    C_Iter<int> c_iter;
    Cpp_Iter<int> cpp_iter;

    iter(java_iter);
    iter(c_iter);
    iter(cpp_iter);
}
