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

void a() {
    std::cout << "outer a" << std::endl;
}

void b() {
    std::cout << "outer b" << std::endl;
}

void free_Child() {
    std::cout << "outer free" << std::endl;
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

