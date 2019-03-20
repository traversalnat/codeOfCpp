#include<iostream>

// 结构体 （属性默认为 public 的类, 同样可以有构造函数和成员函数)
//
struct Vector_struct {
	int sz;
	double* elem;
};

// new 关键字类似 malloc 的效果
void Vector_init(Vector_struct& v, int s) {
	v.elem = new double[s];
	v.sz = s;
}

// class 用户自定义类型, 表示形式与操作的紧密联系
//
class Vector {
	public:
		// 在 构造函数后 :elem... 用于初始化成员变量
		Vector(int s) :elem{new double[s]}, sz{s}
		{
			// 构造函数用于初始化
		}
		// operator 关键字用于定义用户自定义类型的操作
		// operator[] 用于使类可以通过下标访问元素
		// 类似的, 可以定义 operator++, 或operator-- 等操作
		// double& 返回一个引用
		double& operator[] (int i) { return elem[i]; }
		int size() { return sz; }
		
		void printAll();
	private:
		double* elem;	// 元素指针
		int sz;			// 元素数量
};

// 类的成员函数与类的定义分离有助于模块化编译
void Vector::printAll() {
	for (int i = 0; i < sz; i++) {
		std::cout << elem[i];
	}
}

// 枚举 分为强类型枚举(枚举单独为一个类型), 
// 和非强类型枚举(兼容C, 可隐式转换为int 类型)
//
// 强类型枚举 带class , 枚举变量在指定的作用域(花括号中)
// 强类型枚举默认操作符为 == 和 <, 可自定义操作符(使用关键字 operator)
enum class Color {red, blue, green};
// operator++ 前缀形式, 与正常的内置前缀++保持一致
// 枚举类型的运算符重载, 不能作为成员函数
Color& operator++ (Color& t) {
	switch(t) {
		// 强类型枚举的变量有作用域(自己的命名空间)
		case Color::red :
			return t=Color::blue;
		case Color::blue :
			return t=Color::green;
		case Color::green :
			return t=Color::red;
	}
}
// 后缀形式, 返回一个项目副本, int x 为一个虚参数, 扩展为t++x从而实现t++
Color operator++ (Color& t, int x) {
	switch(t) {
		// 强类型枚举的变量有作用域(自己的命名空间)
		case Color::red :
			t=Color::blue;
			return Color::red;
		case Color::blue :
			t=Color::green;
			return Color::blue;
		case Color::green :
			t=Color::red;
			return Color::green;
	}
}

// 非强类型枚举与C 语言中枚举类似, 默认赋值为 0,1,2
// 并且在定义该枚举变量的空间中可见
enum Traffic_light {green, yellow, red};

// C++ 中的 union 联合结构与 C 语言的联合结构类似
enum Type {str, num};
struct Entry {
	char *name;
	Type t;
	union Value {
		int i;	// 如果 t==num 选择 i
		char *s;	// 如果 t==str 选择 s
	};
};



int main() {
	// 类的使用
	Vector v(6);	//得到一个句柄(类的对象)
	double array[6] = {1, 2, 3, 4, 5, 6};
	for (int i = 0; i < v.size(); i++) {
		// 可以看到类 Vector  的对象可以使用下表访问元素
		v[i] = array[i];
	}
	v.printAll();
	std::cout << "\n";
	
	// 枚举变量的使用
	// 强类型枚举需要用到命名空间, 且不能被显示或隐式转换为int 
	Color y = Color::red;
	Color x = y++;	// ++ 前缀和后缀应该与常识相关
	if (x == Color::blue) {
		std::cout << "The color of x is blue\n"; 
	} else if (x == Color::red) {
		std::cout << "The color of x is red\n"; 
	}

	// 非强类型枚举可以当做int 型数据使用
	// 此处Traffic_light 为全局变量
	for (int i = green; i <= red; i++) 
		std::cout << ++i << "\n";
}
