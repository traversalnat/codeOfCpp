#include <gmock/gmock-matchers.h>
#include <gtest/gtest-death-test.h>
#include <gtest/gtest-param-test.h>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>

#include <functional>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

using testing::Test;
class Reference_Wrapper : public Test {};

template <class _Tp>
class reference_wrapper {
 public:
    // types
    typedef _Tp type;

 private:
    type* __f_;

 public:
    // construct/copy/destroy
    reference_wrapper(type& __f) : __f_(_VSTD::addressof(__f)) {}
    // access
    operator type&() const { return *__f_; }
    type& get() const { return *__f_; }

    template <class... _ArgTypes>
    typename std::__invoke_of<type&, _ArgTypes...>::type operator()(
        _ArgTypes&&... __args) const {
        return std::__invoke(get(), std::forward<_ArgTypes>(__args)...);
    }
};

template <class T>
reference_wrapper<T> ref(T& t) {
    return reference_wrapper<T>(t);
}

template <class T>
reference_wrapper<const T> cref(const T& t) {
    return reference_wrapper<const T>(t);
}

void modify(int& x) {
    x = 3;
}

TEST_F(Reference_Wrapper, reference_wrapper_test) { 
    int a = 2;
    auto b = ref(modify);
    b(a);
    std::cout << a << std::endl;
}
