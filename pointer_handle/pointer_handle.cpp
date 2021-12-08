#include <gmock/gmock-matchers.h>
#include <gtest/gtest-death-test.h>
#include <gtest/gtest-param-test.h>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>

#include <iostream>
#include <string>
#include <vector>
using std::string;
using std::cout;
using std::endl;

template<typename T>
class Handle {
  public:
    Handle(T* t = nullptr): _ptr {t}, _used {new size_t{1}} {}
    Handle(const Handle& h): _ptr {h._ptr}, _used {h._used} {
      ++*_used;
    }

    ~Handle() {
      _decUsed();
    }

    // 为了满足能够被 Handle, const Handle 类型调用, -> 重载必须有两个版本:
    // 一个 const 版本, 一个非const 版本
    const T* operator->() const {
      return _ptr;
    }

    T* operator->() {
      return _ptr;
    }

    inline Handle operator=(const Handle &h) {
      ++(*(h._used));
      _decUsed();
      _ptr = h._ptr;
      _used = h._used;
      return *this;
    }

  private:
    T *_ptr;
    size_t *_used;
    void _decUsed() {
      if (--(*_used) == 0) {
        delete _ptr;
        delete _used;
        _ptr = nullptr;
        _used = nullptr;
      }
    }
};

class Data {
  public:
    Data(const string& str) : name {str} {}
    ~Data() {
      cout << "released: " << name << endl;
    }
    void rename(const string& str) {
      name = str;
    }
  private:
    string name;
};

using testing::Test;
class Pointer_Handle : public Test {
};

TEST_F(Pointer_Handle, pointer_handle_test) {
  Handle<Data> data {new Data("data")};
  Handle<Data> h_data;
  h_data = data;
  data->rename("Data");
}

