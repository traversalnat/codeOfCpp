#include <gmock/gmock-matchers.h>
#include <gtest/gtest-death-test.h>
#include <gtest/gtest-param-test.h>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>

#include <iostream>
#include <string>
#include <vector>
using std::string;

using testing::Test;
class Type_Convert : public Test {};

// 通过构造函数实现自动类型转换 (转入 A <= B)
// 通过运算符重载实现自动类型转换 (转出 A => B)

class MobileCar {
 public:
    MobileCar(const string& str) : name{str} {}

 public:
    string name;
};

// 假定 AutoCar 和 Car 是同级的, MobileCar 可以转换成任意一个
// 假定 Car 可以转成 MobileCar

class AutoCar {
  public:
    AutoCar(const string& str) : name {str} {}
    AutoCar(const MobileCar& m) : name(m.name) {}

  public:
    string name;
};

class Car {
 public:
    Car(const string& str) : name{str} {}
    // 通过构造函数实现自动类型转换 (转入 A <= B)
    Car(const MobileCar& m) : name(m.name) {}

    // 通过运算符重载实现自动类型转换 (转出 A => B)
    operator MobileCar() const {
      return MobileCar {name};
    }

 public:
    string name;
};

void print_car_name(Car car) {
  std::cout << "Car: " << car.name << std::endl;
}

void print_mobile_car_name(MobileCar car) {
  std::cout << "MobileCar: " << car.name << std::endl;
}

/*
 * 选择重载函数时，会自动调用无需类型转换的函数
void print_car_name(MobileCar m_car) {
  std::cout << "MobileCar: " << m_car.name << std::endl;
}
 * 但两个重载函数都需要类型转换时，会导致矛盾
 * Ambiguous, 有歧义的
void print_car_name(AutoCar a_car) {
  std::cout << "AutoCar: " << a_car.name << std::endl;
}
 * 同理, 若实现以下代码，就有两种途径可以将 MobileCar 转换成 Car， 同样是矛盾的
  class MobileCar {
    ...
    operator Car() const {
      return Car {name};
    }
    ...
  }
*/

TEST_F(Type_Convert, type_convert_test) {
  MobileCar mobile_car {"mobile_car"};
  // 隐式调用 Car(const MobileCar& m) 将 mobile_car 转换成 Car 类型
  print_car_name(mobile_car);
  // 相当于 Car {mobile_car}
  print_car_name(Car {mobile_car});

  Car car {"car"};
  // 隐式调用 operator MobileCar() const {} 将 car 转换成 MobileCar 类型
  print_mobile_car_name(car);
}
