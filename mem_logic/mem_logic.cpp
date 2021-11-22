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
        cout << d.name << ": copied by copy constructor to " + name + "\n";
      }
      
      Data(Data &&d) : a {d.a}, name { d.name } {
        cout << d.name << ": moved by move constructor to " + name + "\n";
        d.a = -1;
        d.name = "moved " + d.name;
      }

      ~Data() {
        cout << this->name << ": free by ~Data\n";
      }

      Data& operator=(const Data &d) {
        this->a = d.a;
        this->name = "COPY= " + d.name;
        cout << d.name << ": copied by operator= to " + this->name + "\n";
        return *this;
      }

      // move assignment
      Data& operator=(Data &&d) {
        this->a = d.a;
        this->name = "MOVE= " + d.name;
        cout << this->name << ": moved by operator=\n";
        d.a = -1;
        d.name = nullptr;
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
  // 将传入参数视为 d , 传入参数 d 时会调用复制构造函数得到 d1
  // 函数中的 d 实际上是 d1
  return d;
  // return d 实际上调用了 d1 的移动构造函数(若无，则调用复制构造函数), 返回的是 d2
}

class Mem_Logic : public Test {
};

TEST_F(Mem_Logic, mem_logic_test) {
  // 总结: 
  // 1. 类作为参数传入函数，实际上会复制一个副本（复制构造函数），在函数中对参数的操作对原变量无影响
  // 2. 函数在返回一个类时，会尝试调用移动构造函数（若无则调用复制构造函数）得到返回类的副本并返回, 释放函数中所有不在作用域中的类
  // 3. std::move 调用移动构造函数或移动赋值函数，若无则调用复制构造函数或复制赋值函数 (注意初始化类时一定调用的是构造函数, 不论使用 = 还是 {} )
  Data a {1, "a1"};
  cout << "\n\nset function\n";
  set(a);
  // copy 复制了 a, 并通过移动构造函数将 a 的副本移动至 d2 (若无移动构造函数，则调动复制构造函数), 随后 a 的副本被释放, d2 由于离开作用域, 也被释放
  cout << "\n\ncopy function\n";
  copy(a);

  cout << "\n\ncopy2 function\n";
  // copy 复制了 a, 并通过移动构造函数将 a 的副本移动至 b (即 b 就是 d2, 若无移动构造函数，则调动复制构造函数), 随后 a 的副本被释放
  auto b = copy(a);

  ASSERT_EQ(1, a.a);
  ASSERT_EQ(1, b.a);

  cout << "\n\nmove function\n";
  // std::move 如果没有移动赋值函数, 会调用复制构造函数
  auto b1 = std::move(b);
  ASSERT_EQ(1, b1.a);
  cout << "\n\nfree function\n";
}

