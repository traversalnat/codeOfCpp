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
class Vtable_Test : public Test {};

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
    Child() {}
    void a() { std::cout << "a in Child" << std::endl; }

    void b() { std::cout << "b in Child" << std::endl; }

    ~Child() { std::cout << "free in Child" << std::endl; }
};

class Child2 : public Parent {
 public:
    Child2(int a) : aa{a} {}
    void a() { std::cout << "a in Child2: " << aa << std::endl; }

    void b() { std::cout << "b in Child2" << std::endl; }

    ~Child2() { std::cout << "free in Child2" << std::endl; }

 public:
    int aa;
};

void a() { std::cout << "outer a" << std::endl; }

void b() { std::cout << "outer b" << std::endl; }

void free_Child() { std::cout << "outer free Child" << std::endl; }

void a_this(Child2 *t) { std::cout << "outer a " << t->aa << std::endl; }

Child2 *free_Child2(Child2 *t) {
    std::cout << "outer free Child2" << std::endl;
    return t;
}

TEST_F(Vtable_Test, vtable_test_test) {
    using func_type = void (*)();

    Child child;
    void **vtbl = (void **)*(void **)&child;
    ((func_type)vtbl[0])();

    void **p = new void *[3];
    std::unique_ptr<void *> p_guard(p);

    p[0] = (void *)a;
    p[1] = (void *)b;
    p[2] = (void *)free_Child;

    Child *pc;
    pc = (Child *)&p;
    pc->a();
    pc->b();
    pc->~Child();
}

struct child2_table {
    void **p;
    int aa;
};

TEST_F(Vtable_Test, class_test_test) {
    void **p = new void *[6];
    std::unique_ptr<void *> lk_p{p};

    // 虚函数表
    p[0] = 0;     // 虚指针在类中的偏移
    p[1] = NULL;  // RTII 指针
    p[2] = (void *)a_this;
    p[3] = (void *)b;
    p[4] = (void *)+[]() { std::cout << "free p->p[4]" << std::endl; };
    p[5] = (void *)+[]() { std::cout << "free p->p[5]" << std::endl; };

    // 模拟 Child2 类布局
    struct child2_table *table = new struct child2_table;
    table->p = &p[2];  // 虚函数表指针
    table->aa = 123456;

    Child2 child{0};
    // RTTI 信息存储在一个 void * 指针指向的结构体中, 虚函数表的前一项
    // 没有虚函数表, 则 RTTI （运行时类型识别）无意义
    p[1] = ((void **)(*(void **)&child))[-1];

    Child2 *pc;
    pc = (Child2 *)table;
    pc->a();
    pc->b();

    std::cout << typeid(*pc).name() << std::endl;
    std::cout << typeid(child).name() << std::endl;

    delete pc;
}


class Child3 : public Child, public Child2 {
 public:
    Child3(int a) : Child(), Child2{a} {}

    virtual ~Child3() { std::cout << "free in Child3" << std::endl; }
};

TEST_F(Vtable_Test, multi_inheritance) {
    Child3 child{1};
    // 菱形继承导致定义冲突, 需要指定该函数继承自哪个类（确定访问哪个虚函数表）
    child.Child::a();
    child.Child2::a();

    using func_type = void (*)();

    std::cout << " ======= Function in vtable ======= " << std::endl;

    /*
     * Child3 布局
     * vptr of Child
     * vptr of Child2
     * content of Child2
     *
       {
       <Child> = {Parent = {
          _vptr.Parent = 0x4442e0 <vtable for Child3+16>}, <No data fields>},
       <Child2> = {<Parent> = {
          _vptr.Parent = 0x444310 <vtable for Child3+64>}, aa = 1},
      <No data fields>}
      */

    void **vtbl = (void **)*(void **)&child;
    ((func_type)vtbl[1])();
    ((void (*)(Child *))vtbl[2])(&child);

    void **vtbl2 = (void **)*(void **)((size_t)&child + 8);
    ((func_type)vtbl2[1])();
    ((void (*)(Child3 *))vtbl2[2])(
        &child);  // 调用的析构函数组合了~Child3 ~Child2 ~Child

    std::cout << " ======= Function in vtable ======= " << std::endl;
}

TEST_F(Vtable_Test, multi_inheritance_struct) {
    struct vtable {
        void **p1;
        void **p2;
        int aa;
    } *vtbl = new struct vtable;

    /*
     * Child3 虚函数表 (所有析构函数都最终调用父子类的析构函数组合函数)
        __ZTV6Child3:
            .quad	0               // 偏移 this + 0 (可得到 Child)
            .quad	__ZTI6Child3    // typeinfo
            .quad	__ZN5Child1aEv  // Child.a
            .quad	__ZN5Child1bEv  // Child.b
            .quad	__ZN6Child3D1Ev // 析构函数
            .quad	__ZN6Child3D0Ev // 析构函数
            .quad	-8              // 偏移 this + 8 (可得到 Child2)
            .quad	__ZTI6Child3    // typeinfo
            .quad	__ZN6Child21aEv // Child2.a
            .quad	__ZN6Child21bEv // Child2.b
            .quad	__ZThn8_N6Child3D1Ev    // 析构函数
            .quad	__ZThn8_N6Child3D0Ev    // 析构函数
     */

    vtbl->p1 = new void *[5];
    vtbl->p2 = new void *[4];
    vtbl->aa = 123456;

    void **p = vtbl->p1;
    p[0] = (void *)a;
    p[1] = (void *)b;
    p[2] = (void *)+[]() { std::cout << "free p1->p[2]" << std::endl; };
    p[3] = (void *)+[]() { std::cout << "free p1->p[3]" << std::endl; };
    p = vtbl->p2;
    p[0] = (void *)a_this;
    p[1] = (void *)b;
    p[2] = (void *)+[]() { std::cout << "free p2->p[2]" << std::endl; };
    p[3] = (void *)+[]() { std::cout << "free p2->p[3]" << std::endl; };

    Child3 *child = (Child3 *)vtbl;

    child->Child2::a();  // 无需动态绑定, 自动调用代码区的函数（未访问虚函数表）

    // 需要动态绑定, 调用虚函数表中函数
    Child *child1 = static_cast<Child *>(child);
    child1->a();  // a

    Child2 *child2 = static_cast<Child2 *>(child);
    child2->a();  // a_this

    // 虚函数表中 virtual 析构函数逻辑
    // 1. 每张表多有两个析构函数
    // 2. 所有析构函数最终都调用一个析构函数（组合所有析构函数的逻辑）
    // delete child1;   // p1->p[3]
    // delete child2;   // p2->p[3]
    delete child;  // p1->p[3]
}

// 通过结构体转换访问 private 数据
class private_area {
 private:
    int a;

 public:
    int b;

    private_area(int a, int b) : a{a}, b{b} {}
};

TEST_F(Vtable_Test, class_private_area) {
    private_area p{1, 2};

    struct area {
        // void **p;    // virtual function map:if private_area contains virtual
        // function or derive from other class
        int a;
        int b;
    };

    area *p_area = (area *)&p;
    // std::cout << p.a << std::endl; // private a
    std::cout << p_area->a << std::endl;
    std::cout << p_area->b << std::endl;
}
