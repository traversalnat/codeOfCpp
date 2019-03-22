#include<iostream>
// 三种重要的类:
// 具体类, 抽象类, 类层次结构的类
//
// 具体类:	可以像使用内置类型一样使用它们
namespace Complex {
	class complex {
		double re, im;
	public:
		complex(double r, double i): re{r}, im{i} {}
		complex(double r): re{r}, im{0} {}	// 支持 complex a {2.3}的操作
		complex(): re{0}, im{0} {}	// 无参数构造函数, 默认构造函数

		// 为了效率, 具体类的成员函数通常定义在类的内部, 或者用inline 修饰
		// 使用 const 修饰的函数表示不会修改所调用的对象
		// real() 函数使用const 修饰表示不会修改 re
		double real() const {return re;}
		void real(double d) {re = d;}
		double imag() const {return im;}
		void imag(double d) {im = d;}
		
		// 具体类自定义的运算操作
		// +=, -= 的操作需要具体操作complex 的成员变量, 
		// 通常定义在类的内部
		complex& operator+=(complex z) {
			re += z.re, im += z.im;
			return *this;	// 表示当前类的指针
		}

		complex& operator-=(complex z) {
			re -= z.re, im -= z.im;
			return *this;
		}

		complex& operator*=(complex z) {
			re *= z.re, im *= z.im;
			return *this;
		}
		complex& operator/=(complex z) {
			// 这里没有处理可能的除零异常
			// 事实上c++没有定义 除零异常, 除零会得到 nan
			double divided_num = z.re * z.re + z.im * z.im;
			re = (re * z.re + im * z.im) / divided_num;
			im = (im * z.re - re * z.im) / divided_num;
			return *this;
		}
	};

	// 不需要直接访问成员变量的操作可以定义在类的外部
	complex operator+(complex a, complex b) {return a+=b;}
	complex operator-(complex a, complex b) {return a-=b;}
	complex operator-(complex a) {return {-a.real(), -a.imag()};}
	complex operator*(complex a, complex b) {return a*=b;}
	complex operator/(complex a, complex b) {return a/=b;}

	bool operator==(complex a, complex b) {
		return a.real() == b.real() && a.imag() == b.imag();
	}

	bool operator!=(complex a, complex b) {
		return !(a == b);
	}

	int main() {
		complex a {2.3};
		complex b {a/0};
		complex c {a+b*complex(1, 2.3)};
		std::cout<<b.real()<<" "<<b.imag()<<std::endl;
		std::cout<<c.real()<<" "<<c.imag()<<std::endl;
		return 0;
	}

}


int main() {
	Complex::main();
}
