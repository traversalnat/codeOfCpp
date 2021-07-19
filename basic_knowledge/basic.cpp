#include <gmock/gmock-matchers.h>
#include <gtest/gtest-death-test.h>
#include <gtest/gtest-param-test.h>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>

#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;
using testing::Test;
class BasicTest : public Test {
 public:
    string s;
};

class item {
    string name;
    string type;

 public:
    item(string _name, string _type) : name{_name}, type{_type} {}
    ~item() {}

	bool operator==(const item &a) const {
		return a.name == name && a.type == type;
	}
};

struct Record {
    string name;
    int age;
};

TEST_F(BasicTest, Basic) {
    Record jack{"jack", 21};
    // new will return a ptr
    auto tom = new Record{"tom", 21};

    ASSERT_EQ(jack.age, tom->age);
}

TEST_F(BasicTest, Vector) {
    vector<int> ans{1, 2, 3, 4, 5, 6};
    auto v_vector = new vector<int>({2, 3, 4, 5, 6, 1});
    ASSERT_NE((ans), *v_vector);
}

TEST_F(BasicTest, Reference) {
    using std::cout;
    using std::endl;

    int a = 2;
    int &b = a;  // reference
    int *c = &a;
    ASSERT_EQ(a, b);
    ASSERT_EQ(&a, &b);  // the reference of a is another name of a
    ASSERT_EQ(a, *c);   // the reference is a special pointer
}

TEST_F(BasicTest, ReferenceOfReference) {
    using std::cout;
    int a = 2;
    int &b = a;  // reference
    // the c is rvalue, &&c is the lvalue
    // int &&c = 2; error: lvalue can't be assigned to lvalue
    // int &&c = std::move(a); true: rvalue can be assigned to lvalue
    int &&c = std::move(
        a);  // std::move(a) can't move basic type's value, just copy a to c
    ASSERT_EQ(2, a);
    ASSERT_EQ(2, b);
    ASSERT_EQ(2, c);
}

TEST_F(BasicTest, ReferenceOfPointer) {
    using std::cout;
    int a = 2;
    int *b = &a;
    int *&c = b;
    ASSERT_EQ(a, *c);
}

TEST_F(BasicTest, ReferenceAndPointer) {
    using std::cout;
	item it{"one", "num"};
	item *p = &it;
	ASSERT_EQ(&it, p);
	ASSERT_EQ(*p, it);
}
