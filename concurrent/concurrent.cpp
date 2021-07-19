#include <gmock/gmock-matchers.h>
#include <gtest/gtest-death-test.h>
#include <gtest/gtest-param-test.h>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>

#include <exception>
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <functional>
#include <future>

using testing::Test;
using std::thread;
using std::vector;
using std::ref;
using std::promise;
using std::future;
using std::async;


class Concurrent : public Test {
};

void f(vector<int>& v) {
	for (auto x : v)
		std::cout << x;
	for (auto x : v)
		x++;
}


struct F {
	vector<int>& v;
	explicit F(vector<int>& vv) :v {vv} {}
	void operator()() {
		for (auto x : v)
			std::cout << x;
	}
};

TEST_F(Concurrent, concurrent_test) {
    vector<int> vec{1, 2, 3, 4, 5};
	/* 线程的初始化，将参数复制到线程空间，
	 * 即使函数参数为引用，依然复制，然后在线程空间作为引用使用 
	 * 需要 ref 来显式声明参数为引用。
	 * */
	thread t1 {f, ref(vec)};
	thread t2 {F{vec}};
	t1.join();
	t2.join();
}

void producer(promise<int>& px) {
	try {
		int res = 6;
		px.set_value(res);
	} catch (std::exception_ptr p) {
		px.set_exception(p);
	}
}

double accum(double a, double b) {
	return a + b;
}

TEST_F(Concurrent, futureAndPromise) {
	using Task_type = double(double, double);

	std::packaged_task<Task_type> pt0 {accum};
	std::packaged_task<Task_type> pt1 {accum};
	future<double> f0 {pt0.get_future()};
	future<double> f1 {pt1.get_future()};

	thread t1{std::move(pt0), 5.0, 2.0};
	thread t2{std::move(pt1), 3.0, 4.0};
	t1.join();
	t2.join();

	ASSERT_DOUBLE_EQ(f0.get(), f1.get());
}

TEST_F(Concurrent, async) {
	auto f0 = async(accum, 1.0, 2.0);
	auto f1 = async(accum, 1.0, 2.0);
	ASSERT_DOUBLE_EQ(f0.get(), f1.get());
}
