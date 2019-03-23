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

// 容器类(包含若干元素的对象)
namespace Container {
	class Vector {
		private:
			double *elem;
			int sz;
		public:
			Vector(int s) {
				if (s < 0) {
					throw std::length_error("Vector(s)");
				}
				elem = new double[s];
				sz = s;
				// 资源获取即初始化
				for (int i = 0; i != s; i++) 
					elem[i] = 0;
			}

			// 使用元素列表初始化容器	如Vector v({1.2, 3.4});
			// initializer_list 是一个标准库类型
			// 由编译器识别 列表参数并生产 initializer_list
			Vector(std::initializer_list<double> lst) 
				:elem{new double[lst.size()]}, 
					sz{static_cast<int>(lst.size())}
			{
				const double* p = lst.begin();
				for (int i = 0; i < lst.size(); i++) {
					elem[i] = *(p + i);
				}
			}
			// 容器的标准操作, 将一个元素加入到容器尾部
			// 容器长度加1, 该实现版本效率很低
			void push_back(double x) {
				double *p = new double[sz+1];
				for (int i = 0; i < sz; i++) {
					p[i] = elem[i];
				}
				p[sz] = x;
				delete[] elem;
				elem = p;
				sz += 1;
			}


			// 析构函数, 当类的对象离开作用域的时候, 自动调用析构函数
			// 对局部资源进行释放(类), 确保构造函数分配的内存一定会被释放
			~Vector() {delete[] elem;}

			double& operator[] (int i) {
				if (i < 0 || i > sz) {
					throw std::out_of_range("Vector[]");
				}
				return elem[i];
			}


			int size() const {return sz;}

		/*
		 * 定义在类的外部的形式(同一命名空间内)
			// begin and end 函数, 为了实现foreach 
			const double* begin(Vector& x) {
				return x.size() ? &x[0] : nullptr;
			}

			// 指向末尾元素的下一位置
			const double* end(Vector& x) {
				return begin(x) + x.size();
			}
		*/
			const double* begin() {
				return sz ? &elem[0] : nullptr;
			}

			const double* end() {
				return begin() + sz;
			}
	};


	int main() {
		try {
			Vector v(-1);
		} catch (std::length_error) {
			std::cout<<"Catch length_error\n";
		}

		Vector vc({1.2, 3.4, 4.5});
		for (int i = 0; i < 5; i++) 
			vc.push_back(i + 5.21);
		for (auto x : vc) {
			std::cout<<x<<" ";
		}
		std::cout<<std::endl;
		return 0;
	}
};


int main() {
	Complex::main();
	Container::main();
}
