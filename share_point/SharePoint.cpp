#include <gmock/gmock-matchers.h>
#include <gtest/gtest-death-test.h>
#include <gtest/gtest-param-test.h>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>

#include <iostream>
#include <string>
#include <vector>

class Data {
	public:
		Data(std::string name) 
			: name {name} {
			for (int i = 0; i < 100; i++)
				this -> data.push_back(i);
		}

		~Data() {
			std::cout << name << " has been freed!\n";
		}

		static Data newData(std::string name);

		std::string name;

	private:
		std::vector<int> data;
};

Data Data::newData(std::string name) {
    return Data(name);
}

template<class T>
class share {
	public:
		share(T *ptr) : __ptr__{ptr} {}
		~share() {delete __ptr__;}

		// -> 操作符仅返回__ptr__指针本身
		// share<T> a {new T()}; a->member() 能够实现的条件
		T* operator ->() const {
			return __ptr__;
		}

		// std::shared_ptr 未使用这种做法
		// T t;
		// 存在缺陷: share<T> a = &t;
		share& operator = (T *ptr) {
			// std::shared_ptr<T>;
			__ptr__ = ptr;
			return *this;
		}
	private:
		T *__ptr__;
};

using testing::Test;

class Sharepoint : public Test {
	public:
		share<Data> a {new Data("a")};
		// t will be freed before b
		// you will get error: pointer being freed was not allocated
		Data t {"b"};
		share<Data> b = &t;
};

TEST_F(Sharepoint, SharePoint_test) {
	ASSERT_TRUE(a->name == "a");
	ASSERT_TRUE(b->name == "b");
}

