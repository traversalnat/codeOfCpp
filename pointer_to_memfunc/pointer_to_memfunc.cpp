#include <gmock/gmock-matchers.h>
#include <gtest/gtest-death-test.h>
#include <gtest/gtest-param-test.h>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>

#include <iostream>
#include <string>
#include <vector>

using testing::Test;
using std::string;
using std::cout;
using std::endl;

class Pointer_To_Memfunc : public Test {
};

class Receiver {
  public:
    Receiver(const string &str): name {str} {}
    // 成员函数指针 typedef, 简化使用
    typedef void (Receiver::*slot_ptr) (const string &);
    void slot(const string &str) {
      cout << "Receive: " << str << endl;
    }
  private:
    string name;
};

TEST_F(Pointer_To_Memfunc, pointer_to_memfunc_test) {
  Receiver *r {new Receiver {"Receiver"}};
  // 由于成员函数指针的类型转换限制, 类似qt connect() 的实现需要更多信息
  Receiver::slot_ptr slot_ptr = &Receiver::slot;
  // () 的优先级大于 *
  (r->*slot_ptr)("hello");
}

