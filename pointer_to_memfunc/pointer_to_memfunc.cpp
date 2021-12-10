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
using std::vector;

class Pointer_To_Memfunc : public Test {
};

// 模板类SenderSlot无法直接保存, 需要使用基类
class ISenderSlot {
  public:
    virtual void call() =  0;
    virtual string getKey() = 0;
};

/**
 * @brief 用于保存信号/槽
 *
 * @tparam S 信号类
 * @tparam SIGNAL 信号函数
 * @tparam T 槽类
 * @tparam SLOT 槽函数
 */
template<typename S, typename SIGNAL, typename T, typename SLOT>
class SenderSlot : public ISenderSlot {
  // 用于保存 sender 和 slot 
  public:
    SenderSlot(S *sender, SIGNAL signal, T *receiver, SLOT slot) 
      : sender {sender}, signal {signal}, receiver {receiver}, slot {slot} {}

    void call() {
      (receiver->*slot)();
    }

    /**
     * @brief 返回 信号SIGNAL 的类型
     *
     * @return 
     */
    string getKey() {
      string name {typeid(SIGNAL).name()};
      return name;
    }

  private:
    S *sender;
    SIGNAL signal;
    T *receiver;
    SLOT slot;
};

// 单例处理中心
// 存储 signal 和 slot 之间的关系, 在 signal 发起调用后自动调用 slot 函数
class CENTER {
  private:
    CENTER() {}
    CENTER(const CENTER &) = delete;
    CENTER& operator=(const CENTER &) = delete;
    // 单例模式
    static CENTER *center; 
    /**
     * @brief 暂时使用 vector, 可更改为 map
     */
    vector<ISenderSlot *> senderSlots;
  public:
    ~CENTER() {}

    static CENTER* getCenter() {
      if (center == nullptr) {
        center = new CENTER();
      }

      return center;
    }

    /**
     * @brief 触发 signal 绑定的 slot
     *
     * @tparam SIGNAL
     */
    template<typename SIGNAL>
    void call() {
      /// 从 senderSlots 获取 SIGNAL 对应的所有 SenderSlot, 并调用 call()
      string key {typeid(SIGNAL).name()};
      for (auto iss : senderSlots) {
        if (iss->getKey() == key) {
          iss->call();
        }
      }
    }

    /**
     * @brief 存储 SenderSlot 用于调用
     *
     * @param ss
     */
    void insert(ISenderSlot *ss) {
      senderSlots.push_back(ss);
    }

};

/**
 * @brief 单例模式必须在外部初始化 center
 */
CENTER* CENTER::center = nullptr;

class Sender {
  public:
    Sender(const string &str): name {str} {}
    // 还没想好参数怎么处理
    void signal() {
      typedef void (Sender::*signal_ptr)(string str);
      CENTER::getCenter()->call<signal_ptr>();
    }

    /**
     * @brief 模拟 emit signal 
     */
    void trigger() {
      signal();
    }
  private:
    string name;
};

class Receiver {
  public:
    Receiver(const string &str): name {str} {}
    void slot() {
      cout << name << " has received: " << endl;
    }
  private:
    string name;
};

template<typename S, typename SIGNAL, typename T, typename SLOT>
void connect(S *sender, SIGNAL signal, T *receiver, SLOT slot) {
  SenderSlot<S, SIGNAL, T, SLOT> *ss = new SenderSlot<S, SIGNAL, T, SLOT> {sender, signal, receiver, slot};
  CENTER::getCenter()->insert(ss);
}

TEST_F(Pointer_To_Memfunc, pointer_to_memfunc_test) {
  Receiver *r {new Receiver {"Receiver"}};
  Sender *s {new Sender {"Sender"}};
  // 借助模板的自动推导, 不用设定类型
  connect(s, &Sender::signal, r, &Receiver::slot);
  // 模拟 emit signal
  s->trigger();
}

void my_print() {}
template<typename Head, typename... Args>
void my_print(Head head, Args... args) {
  cout << head; 
  my_print(args...);
}

TEST_F(Pointer_To_Memfunc, varible_parameters) {
  my_print("123", 2, "\n");
}
