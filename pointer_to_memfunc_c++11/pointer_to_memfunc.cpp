#include "pointer_to_memfunc.h"

#include <gmock/gmock-matchers.h>
#include <gtest/gtest-death-test.h>
#include <gtest/gtest-param-test.h>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>


#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <any>
#include <memory>

#include "../any_object/any.h"

using std::cout;
using std::endl;
using std::function;
using std::string;
using std::vector;
using testing::Test;
using hlp::any;
using hlp::SRObject;
using hlp::connect;

class Pointer_To_Memfunc : public Test {};

class Receiver : public SRObject {
 public:
    Receiver(const string &str) : name{str} {}
    void slot(string str) { cout << name << " has received: " << str << endl; }
    void get_slot(string str, int num) {
        cout << name << " has received: " << num << " " << str << endl;
    }

 private:
    string name;
};

class Sender : public SRObject {
 public:
    Sender(const string &str) : name{str} {}

    void signal(string str) { PUBLIC_SIGNAL(Sender, signal, str); }

    void push_signal(string str, int num) {
        PUBLIC_SIGNAL(Sender, push_signal, str, num);
    }

    /**
     * @brief 模拟 emit signal
     */
    void trigger() {
        signal("你好, 世界");
        push_signal("推送的邮件", 3);
    }

 private:
    string name;
};

TEST_F(Pointer_To_Memfunc, pointer_to_memfunc_test) {
    Receiver *r{new Receiver{"Receiver"}};
    Sender *s{new Sender{"Sender"}};
    connect(s, &Sender::signal, r, &Receiver::slot);
    connect(s, &Sender::push_signal, r, &Receiver::get_slot);
    // 模拟 emit signal
    s->trigger();
}
