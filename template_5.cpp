#include<iostream>
#include<stdexcept>

// 模板, 用一组类型或值对其参数化的一个类或对象

// 1. 参数化类型
// 模板是一种编译时的机制, 并不会产生额外的运行时开销
// 模板也可以使用普通的值参数, 如 Buffer<int, 10> buf;
// 即可以使用模板定义一个指定类型的局部数组 T buf[size];
namespace template_1 {
	template<typename T>
	class Vector {
		private:
			T* elem;
			int sz;
		public:
			// explicit 关键字修饰构造函数, 禁止隐式类型转换
			// 表示禁止使用 Vector v = 7; 来表示 Vector v(7);
			// 但是隐式转换是有用的, 如 complex v = 7; 效果为
			// complex v {7, 0}, 可以使得像 complex 这样的类型
			// 更加像是内部类型
			explicit Vector(int s)		// 构造函数: 建立不变式, 获取资源
			{
				if (s < 0) {
					throw "Negative size";
				}
				elem = new T[s];
				sz = s;
			}
			~Vector() {delete[] elem;}	// 析构函数: 释放资源

			T& operator[](int i) {
				if (i < 0 || size() <= i) {
					throw std::out_of_range{"Vector::operator[]"};
				}
				return elem[i];
			}
			int size() const {return sz;}
			// 定义 begin 和 end 使得 Vector 支持 foreach
			T* begin() {
				return sz ? &elem[0] : nullptr;
			}

			T* end() {
				return begin() + sz;
			}
	};


	// 模板也可以接受普通的值参数
	// 实现 C 中 
	// #define MAXSIZE 100 
	// int buf[MAXSIZE];的效果
	// N 必须是 constexpr
	template<typename T, int N>
	class Buffer {
		private:
			T elem[N];
		public:
		Buffer() {}
		// 需要只读的返回模板参数 N
		int size() const {return N;}
		T& operator[](int i) {
			return elem[i];
		}
	};


	int main() {
		Vector<char> v {20};
		for (auto& c : v) {
			c = 'x';
		}

		for (auto& c : v) {
			std::cout<<c;
		}
		std::cout<<"\n";

		Buffer<char, 20> buf;
		for (int i = 0; i < 20; i++) {
			buf[i] = 'x';
		}
		for (int i = 0; i < 20; i++) {
			std::cout<<buf[i];
		}
		std::cout<<"\n";

		return 0;
	}
};

// 2. 函数模板
namespace template_2 {
	template<typename Container, typename Value>
		Value sum(Container& c, Value v) {
			// 使用const 修饰Container& c的时候
			// Container 中的成员函数都必须使用 const 修饰
			// 所以一般情况下复杂类型参数不建议使用 const修饰
			for (auto x : c) {
				v += x;
			}
			return v;
		}

	int main() {
		template_1::Vector<double> ld(3);
		for (auto& x : ld) 
			x = 9.2;
		double vi = 0.;
		std::cout<<template_2::sum<template_1::Vector<double>, double>(ld, vi)<<"\n";
		for (auto& x : ld) 
			std::cout<<x<<" ";
		std::cout<<"\n";
		return 0;
	}
};

int main() {
	template_1::main();
	template_2::main();
	return 0;
}
