#include <gmock/gmock-matchers.h>
#include <gtest/gtest-death-test.h>
#include <gtest/gtest-param-test.h>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>

#include <iostream>
#include <iterator>
#include <string>
#include <vector>

using testing::Test;
using namespace std;
class Adaptor : public Test {};

template <class T>
void print_vector(T t) {
    ostream_iterator<typename T::value_type> outite(std::cout, " ");
    copy(t.begin(), t.end(), outite);
    std::cout << std::endl;
}

TEST_F(Adaptor, adaptor_test) {
    vector<int> ver{4, 3, 4, 5, 1, 2, 5};
    vector<int> ver_copy{ver.begin(), ver.end()};
    print_vector(ver);

    // inserter，将 vector 改造为具有 insert 操作的 iterator
    // vte 是 vector 的中间点
    auto vte = std::find(ver.begin(), ver.end(), 3);

    std::copy(ver_copy.begin(), ver_copy.end(), inserter(ver, vte));
    print_vector(ver);

    // back_inserter，将 vector 改造为具有 push_back 操作的 iterator
    copy(ver_copy.begin(), ver_copy.end(), back_inserter(ver));
    print_vector(ver);

    vector<string> vstring{"1", "abc", "*()"};
    print_vector(vstring);

    // istream_iterator 的用法, 可以将 istringstream 换成 cin
    istringstream str{"1 2 3 4 5 6 7 8 9 0"};
    istream_iterator<int> readint(str), eos;
    copy(readint, eos, back_inserter(ver));
    print_vector(ver);

    // istringstream 读取后会被置空，这里新建一个 istringstream
    istringstream char_str{"1 2 3 4 5 6 7 8 9 0"};
    istream_iterator<char> readchar(char_str), eoc;
    vector<char> vchar{};
    copy(readchar, eoc, back_inserter(vchar));
    print_vector(vchar);
}

template <class Operation1, class Operation2>
class unary_compose : public unary_function<typename Operation2::argument_type,
                                            typename Operation1::result_type> {
 protected:
     Operation1 op1;
     Operation2 op2;

 public:
     unary_compose(const Operation1& x, const Operation2& y)
         : op1 {x}, op2 {y} {}

     typename Operation1::result_type
     operator() (const typename Operation2::argument_type& x) const {
         return op1(op2(x));
     }
};

template <class Operation1, class Operation2>
inline unary_compose<Operation1, Operation2>
compose1(const Operation1& x, const Operation2& y) {
    return unary_compose<Operation1, Operation2>(x, y);
}

int func1(int x) {
    return x + 1;
}

int func2(int x) {
    return x * 2;
}

TEST_F(Adaptor, function_adaptor) {
    std::vector<int> ver{1, 2, 3, 12, 13, 14};
    auto num = std::count_if(ver.begin(), ver.end(),
                             bind(less<int>(), std::placeholders::_1, 12));
    std::cout << num << std::endl;

    // ptr_fun is deprecated in c++11 
    std::cout << compose1(function<int(int)>(func1), function<int(int)>(func2))(3) << std::endl;

    // compose1 is not in stl 
    // use bind instead

    auto func3 = [](int x) {
        return func1(func2(x));
    };
    std::cout << func3(3) << std::endl;
}

