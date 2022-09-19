#include <gmock/gmock-matchers.h>
#include <gtest/gtest-death-test.h>
#include <gtest/gtest-param-test.h>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using testing::Test;
class Vtable_Test : public Test {
};

class Parent {
public:
    virtual void a() = 0;
    virtual void b() = 0;
    virtual ~Parent() = default;

protected:
    Parent() = default;
};

class Child : public Parent {
public:
    Child() {

    }
    void a() {
        std::cout << "a in Child" << std::endl;
    }

    void b() {
        std::cout << "b in Child" << std::endl;
    }

    ~Child() {
        std::cout << "free Child" << std::endl;
    }
};

class Child2 : public Parent {
public:
    Child2(int a) : aa{a} {}
    void a() {
        std::cout << "a in Child2" << std::endl;
    }

    void b() {
        std::cout << "b in Child2" << std::endl;
    }

    ~Child2() {
        std::cout << "free in Child2" << std::endl;
    }

public:
    int aa;
};

void a() {
    std::cout << "outer a" << std::endl;
}

void b() {
    std::cout << "outer b" << std::endl;
}

void free_Child() {
    std::cout << "outer free" << std::endl;
}


void a_this(Child2 *t) {
    std::cout << "outer a " << t->aa <<std::endl;
}

Child2* free_Child2(Child2 *t) {
    std::cout << "outer free" << std::endl;
    return t;
}

TEST_F(Vtable_Test, vtable_test_test) {
    using func_type = void (*)();

    Child child;
    void **vtbl = (void **)*(void **)&child;
    ((func_type) vtbl[0])();

    void **p = new void*[3];
    std::unique_ptr<void *> p_guard(p);

    p[0] = (void *)a;
    p[1] = (void *) b;
    p[2] = (void *) free_Child;

    Child *pc;
    pc = (Child *) &p;
    pc->a();
    pc->b();
    pc->~Child();
}

struct child2_table {
    void **p;
    int aa;
};

TEST_F(Vtable_Test, class_test_test) {
    void **p = new void*[6];
    std::unique_ptr<void*> lk_p {p};

    // 虚函数表
    p[0] = 0;       // 虚指针在类中的偏移
    p[1] = NULL;    // RTII 指针
    p[2] = (void *) a_this;
    p[3] = (void *) b;
    p[4] = (void *) free_Child2;
    p[5] = nullptr; // 虚函数表结束

    // 模拟 Child2 类布局
    struct child2_table *table = new struct child2_table;
    table->p = &p[2];   // 虚函数表指针
    table->aa = 123456;

    Child2 child {0};
    // RTTI 信息存储在一个 void * 指针指向的结构体中, 虚函数表的前一项
    // 没有虚函数表, 则 RTTI （运行时类型识别）无意义
    p[1] = ((void **)(*(void **)&child))[-1];   

    Child2 *pc;
    pc = (Child2 *) table;
    pc->a();
    pc->b();

    std::cout << typeid(*pc).name() << std::endl;
    std::cout << typeid(child).name() << std::endl;

    // delete pc;
}


// 通过结构体转换访问 private 数据
class private_area {
    private:
        int a;
    public:
        int b;

        private_area(int a, int b) : a {a}, b {b} {

        }
};

TEST_F(Vtable_Test, class_private_area) {
    private_area p { 1,2 };

    struct area {
        // void **p;    // virtual function map:if private_area contains virtual function or derive from other class
        int a;
        int b;
    };

    area *p_area = (area *)&p;
    // std::cout << p.a << std::endl; // private a
    std::cout << p_area->a << std::endl;
    std::cout << p_area->b << std::endl;
}
