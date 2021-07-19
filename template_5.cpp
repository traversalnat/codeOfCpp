#include<iostream>
#include<stdexcept>
#include<vector>

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
		explicit Vector(int s) {	// 构造函数: 建立不变式, 获取资源
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
		std::cout << c;
	}
	std::cout << "\n";

	Buffer<char, 20> buf;
	for (int i = 0; i < 20; i++) {
		buf[i] = 'x';
	}
	for (int i = 0; i < 20; i++) {
		std::cout << buf[i];
	}
	std::cout << "\n";

	return 0;
}
};  // namespace template_1

// 2. 函数模板
namespace template_2 {
template <typename Container, typename Value>
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
    for (auto& x : ld) x = 9.2;
    double vi = 0.;
    std::cout << template_2::sum<template_1::Vector<double>, double>(ld, vi)
              << "\n";
    for (auto& x : ld) std::cout << x << " ";
    std::cout << "\n";
    return 0;
}
};  // namespace template_2

// 3. 函数对象
namespace template_3 {
template <typename T>
class Less_than {
    const T val;

 public:
    explicit Less_than(const T& v) : val{v} {}
    // 重载 () 函数调用运算符
    bool operator()(const T& v) const { return v < val; }
};

// 作为算法的参数出现
// pred 是一个重载 () 函数调用运算符的一个对象
// 可以称之为函数对象
template <typename C, typename P>
// 对复杂类型慎用 const
int count(C& c, P pred) {
    int cnt = 0;
    for (auto& x : c) {
        if (pred(x)) {
            ++cnt;
        }
    }
    return cnt;
}

int main() {
    Less_than<int> lti{142};
    Less_than<std::string> lts{"Backus"};
    bool b1 = lti(141);
    bool b2 = lts("Back");
    if (b1) {
        std::cout << "b1" << std::endl;
    }
    if (b2) {
        std::cout << "b2" << std::endl;
    }

    // 使用函数对象
    template_1::Vector<int> vec(3);
    for (auto& x : vec) x = 141;
    auto cnt = count(vec, lti);
    std::cout << cnt << std::endl;

    // 使用 lambda 表达式生成函数对象
    // 使用方式 []() {}
    // [] 中传递的是对 {} 所使用的局部变量的
	// [&] 希望对所有局部变量使用引用访问,
    // [=] 希望所有局部变量使用值访问,
    // [&x] 希望对特定的变量x 使用引用访问
	// [=x] 希望对特定的变量x 使用值访问
	// [] 不适用任何局部变量
    int cmp = 143;
    std::cout << count(vec, [&](int a) { return a < cmp; }) << "\n";
    return 0;
}
};  // namespace template_3

namespace template_4 {
template <typename T>
void g(T& x) {
    std::cout << x;
}
// 可变参数模板, 无参数函数 f, 用于当 f(tail...) 中tail无参数的时候
// f 使用可变参数, 在这里实现 printf 的效果
void f() { std::cout << std::endl; }
template <typename T, typename... Tail>
void f(T head, Tail... tail) {
    // 注意第一个参数和剩下的参数的区别
    g(head);
    // ... 表示列表中剩余的部分, 使用递归逐步取出tail中参数
    f(tail...);
}

// 这是综合可变长参数和函数对象的一个实现
// sep 即可打印的元素之间执行的操作, 用户自定义
template <typename P>
void print(P sep) {}
template <typename P, typename T, typename... Tail>
void print(P sep, T head, Tail... tail) {
    g(head);
    sep();
    print(sep, tail...);
}

// 使用字符串作为 sep
void printf(std::string sep) {}
template <typename T, typename... Tail>
void printf(std::string sep, T head, Tail... tail) {
    g(head);
    std::cout << sep;
    printf(sep, tail...);
}

int main() {
    f(1, 2, 3, 4, "hello");
    print([]() { std::cout << "\n"; }, 1, 2, 3, 4, 5);
    printf("\n", 1, 2, 3, 4, "hello");
    return 0;
}
};  // namespace template_4

// 别名的使用 类似 typedef
// 如 using size_t = unsigned int; 类似于 typedef unsigned int size_t;
// 每个标准容器都使用 value_type做为其类型的名字
// Element_type 使用模板, 使用方式为 Element_type<typename>
// typename C 为一个标准容器
//


template<typename C>
using Element_type = typename C::value_type;

template<typename Container>
void algo(Container &c) {
	std::vector<Element_type<Container>> vec;
	for (auto &x : c) {
		vec.push_back(x);
	}

	for (auto &x : vec) {
		std::cout << x;
	}
    std::cout << "\n";
}
void testAlgo() {
    std::vector<int> vec({1, 2, 3, 4});
    algo(vec);
}

// 通过别名绑定模板的部分实参, 定义新的模板
template<typename Key, typename Value>
class Map {
	public:
		Map() {}
};
template<typename Value>
using String_map = Map<std::string, Value>;

int main() {
	template_1::main();
	template_2::main();
	template_3::main();
	template_4::main();
	testAlgo();
	return 0;
}
