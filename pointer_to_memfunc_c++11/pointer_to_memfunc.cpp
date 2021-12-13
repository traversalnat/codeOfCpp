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

using hlp::any;
using hlp::any_cast;
using std::cout;
using std::endl;
using std::function;
using std::string;
using std::unordered_map;
using std::vector;
using testing::Test;

class Pointer_To_Memfunc : public Test {};

/**
 * @brief 信号/槽函数类
 */
class SRObject {
 public:
    /**
     * @brief 将 sig 信号/ slot 槽键值对插入 slot_map
     *
     * @param sig
     * @param slot
     */
    void _bind_signal(const string &sig, any slot) {
        slot_map[sig].push_back(slot);
    }

 protected:
    /**
     * @brief signal/slot 键值对
     */
    unordered_map<string, vector<any>> slot_map;
};

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

template <typename S, typename SIGNAL, typename T, typename SLOT>
void connect(S *sender, SIGNAL signal, T *receiver, SLOT slot) {
    function<typename bare_func<SLOT>::type> slot_func = binded_mem_fn(receiver, slot);
    any slot_any = slot_func;
    sender->_bind_signal(typeid(SIGNAL).name(), slot_any);
}

TEST_F(Pointer_To_Memfunc, pointer_to_memfunc_test) {
    Receiver *r{new Receiver{"Receiver"}};
    Sender *s{new Sender{"Sender"}};
    connect(s, &Sender::signal, r, &Receiver::slot);
    connect(s, &Sender::push_signal, r, &Receiver::get_slot);
    // 模拟 emit signal
    s->trigger();
}
