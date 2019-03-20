#include<iostream>
// 标准异常库
#include<stdexcept>
// 分离编译(声明与实现分离), 命名空间, 错误处理(异常,不变式,静态断言)

// 分离编译(声明与实现分离), 和错误处理机制
// Vector.h
class Vector {
	public:
		Vector(int s);
		double& operator[](int i);
		int size() noexcept;

	private:
		double* elem;
		int sz;
};

//Vector.cpp
//#include"Vector.h"

/*
 * 初始化有一个问题: s 是否合法?
Vector::Vector(int s)
	:elem {new double[s]}, sz{s}
{

}
*/

Vector::Vector(int s) {
	// 首先建立不变式, 即假定某事为真, 构造函数要确保前提条件为真
	// 在这里不变式为 s > 0, 由构造函数确保s > 0, 从而使得成员函数可以
	// 确定该事为真
	if (s<0) {
		// 抛出异常
		throw std::length_error("Vector::Vector()");
	}
	// new 操作同样会在内存耗尽的时候抛出 std::bad_alloc
	elem = new double[s];
	sz = s;
}

/*
 * 没有检查可能的越界访问行为
double& Vector::operator[] (int i) {
	return elem[i];
}
*/

double& Vector::operator[] (int i) {
	if (i < 0 || size() < i) {
		// 如果抛出的异常没有被catch 处理, 会显示给定的异常信息
		throw std::out_of_range("Vector::operator[]");
	}
	return elem[i];
}

// 在永远不会抛出异常的函数声明为 noexcept
// 但是一旦发生异常, 标准库函数 terminate() 会终止当前程序
int Vector::size() noexcept {
	return sz;
}

// 异常处理测试
void test() {
	try {
		Vector(-26);
	} catch(std::length_error) {
		std::cout<<"Test failed: length error\n";
		// 重新抛出
		throw;
	} catch (std::bad_alloc) {
		// 终止程序
		std::terminate();
	}
}


// user.cpp
using namespace std;
double sqrt_sum(Vector& v) {
	double sum = 0;
	for (int i = 0; i < v.size(); i++) {
		sum += v[i];
	}
	return sum;
}

// 定义命名空间 My_code
// 命名空间, 每个命名空间里的名字与std 空间不会冲突
// 包括 函数main
namespace My_code {
	int main();
}
int My_code::main() {
	Vector v(2);
	for (int i = 0; i < v.size(); i++) {
		v[i] = i+2;
	}
	std::cout<<sqrt_sum(v)<<endl;
	return 0;
}


int main() {
	Vector v(2);
	for (int i = 0; i < v.size(); i++) {
		v[i] = i;
	}
	cout<<sqrt_sum(v)<<endl;
	My_code::main();
	// 异常测试
	try {
		test();
	} catch(std::length_error) {
		cout<<"Catch length_error in main()\n";
	}

	// 静态断言
	// 能够在编译时确定的错误, 通常为泛型的形参类型检查, 以及常量表达式
	// 作为手脚架使用
	// 常量表达式能够在编译时求值, 故要求使用constexpr
	constexpr double pi = 3.14159;
	constexpr double my_pi = 3.14 * 1.002;
	static_assert(my_pi>pi, "my_pi is larger than pi\n");
}

