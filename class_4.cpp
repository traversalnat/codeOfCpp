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

// 容器类(包含若干元素的对象, 具体类的一种)
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

// 抽象类(分离接口和类)
namespace Virtual {
	class Container {
		// Container 接口, Vector 更抽象的版本
		public:
			//virtual 表示虚函数, 虚函数 = 0表示纯虚函数
			virtual double& operator[](int) = 0;
			virtual int size() const = 0;
			virtual ~Container() {}
	};

	// :public 派生自 的含义
	class Vector_container : public Container {
		// 命名空间与内部类重名, 在命名空间前加 ::
		::Container::Vector v;
		public:
			Vector_container(int s) : v(s) {}
			~Vector_container() {}

			// override 表示显示覆盖, 如果父类没有对应的虚函数
			// 将报错, 防止没有覆盖某些虚函数
			double& operator[](int i) override {
				return v[i];
			}
			int size() const override {
				return v.size();
			}

			// 自定义成员函数
			void swink() {
				std::cout<<"Vector_container.swink\n";
			}
	};

	// 接口的使用和java 类似
	void use(Container& c) {
		const int sz = c.size();
		for (int i = 0; i != sz; i++) {
			std::cout<< c[i] << " ";
		}
		std::cout<<std::endl;
	}
	int main() {
		// 构造函数的使用, 依然可以使用 {} 直接赋值
		Vector_container v {3};
		for (int i = 0; i < 3; i++) {
			v[i] = i;
		}
		use(v);

		// 使用 dynamic_cast<>() 表示期望得到的对象
		// 如果不是, 则返回nullptr, 即希望得到一个具体类的实例
		Container* p = &v;
		if (Vector_container* q = dynamic_cast<Vector_container*>(p)) {
			q -> swink();
		} else {
			std::cout<<"Can't swink!\n";
		}
		return 0;
	}
};	

// 拷贝和移动, 关于类的操作
namespace Operators {
	class Vector {
		private:
			double *elem;
			int sz;
		public:
			Vector(int s) :elem {new double[s]}, sz {s} {

			}

			// 拷贝构造函数
			Vector(const Vector& x)
				: elem {new double[x.sz]}, sz {x.sz} {
					for (int i = 0; i < x.sz; i++) {
						elem[i] = x.elem[i];
					}
			}

			// 移动函数
			// 如何与拷贝构造函数区分开来??
			// 可以使用 std::move(x), 如 Vector y(std::move(x))
			// 来告诉编译器使用右值引用(操作等号右边的变量)
			Vector(Vector&& x) : elem {x.elem}, sz {x.sz} {
				x.elem = nullptr;
				x.sz = 0;
			}
			
			~Vector() {delete[] elem;}


			double& operator[] (int i) {
				if (i < 0 || i > sz) {
					throw std::out_of_range("Vector::operator[]");
				}
				return elem[i];
			}

			// 赋值拷贝函数
			Vector& operator=(const Vector& x) {
				double *p = new double[x.sz];
				for (int i = 0; i < x.sz; i++) {
					p[i] = x.elem[i];
				}
				delete[] elem;
				elem = p;
				sz = x.sz;
				return *this;
			}

			// 有些时候, 我们不希望用到拷贝或者移动构造函数, 或其它操作
			// 可以使用 = delete; 表示抑制该操作
			// 如
			// Vector(Vector&) = delete;

			int size() const {return sz;}
	};



	int main() {
		Vector x(2);
		x[0] = 1;
		x[1] = 1;
		Vector y(std::move(x));
		y[0] = 2;
		std::cout<<x.size()<<"\n";
		return 0;
	}

};


int main() {
	Complex::main();
	Container::main();
	Virtual::main();
	Operators::main();
}
