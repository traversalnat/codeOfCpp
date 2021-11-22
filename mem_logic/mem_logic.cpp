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
using std::to_string;

static int name_index = 1;

class Data {
public:
      Data(int a, string name) : a {a}, name {name} {
        cout << name << ": init\n";
      }

      Data(const Data &d) : a {d.a}, name { to_string(name_index) + d.name} {
        name_index += 1;
        cout << name << ": copied by copy constructor\n";
      }

      ~Data() {
        cout << this->name << ": free by ~Data\n";
      }

      Data& operator=(Data &d) {
        this->a = d.a;
        this->name = "COPY= " + d.name;
        cout << this->name << ": copied by operator=\n";
        return *this;
      }
 
public:
      int a;
      string name;
};

void set(Data d) {
  // 传入参数, 调用复制构造函数, d 是传入参数的副本
  d.a = 2;
  d.name = "SET " + d.name;
  // 在此触发 d 的析构函数
}

Data copy(Data d) {
  // 实际上返回的是传入参数通过复制构造函数复制的副本
  return d;
  // 由于 return, 在此不会触发 d 的析构函数
}

class Mem_Logic : public Test {
};

TEST_F(Mem_Logic, mem_logic_test) {
  Data a {1, "a1"};
  cout << "\n\nset function\n";
  set(a);
  // 由于没有变量接收 copy 返回值, 在此处会触发 copy 返回值的析构函数
  cout << "\n\ncopy function\n";
  copy(a);

  cout << "\n\ncopy2 function\n";
  // 变量 b 通过复制构造函数获得 copy 返回值的副本
  // = 号触发了复制构造函数
  auto b = copy(a);

  cout << "\n\nfree function\n";
  ASSERT_EQ(1, a.a);
  ASSERT_EQ(1, b.a);
}

