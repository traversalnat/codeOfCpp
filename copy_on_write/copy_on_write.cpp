#include <gmock/gmock-matchers.h>
#include <gtest/gtest-death-test.h>
#include <gtest/gtest-param-test.h>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>

#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::ostream;
using std::string;
using testing::Test;

class Copy_On_Write : public Test {};

// 模拟写时复制的深拷贝优化技术 (用到引用计数)
/* 在拷贝一个类的时候，对于其中的指针:
 * 浅拷贝只拷贝其中的指针
 * 深拷贝拷贝指针对应的数据
 * 写时复制要做到未对指针对应的数据进行操作时只进行浅拷贝
 * 在操作指针数据前才真正深拷贝
 */


/**
 * @brief 写时拷贝实验
 * 假设只有一种情况设计写操作，就是使用[] 操作符获得 String 中字符的引用
 * _unalias 函数用于深度拷贝
 * _decUsed 函数负责减少引用计数, 且在引用计数为0时释放数据
 */
class String {
 public:
    String()
        : _cstr{nullptr}, _used{new size_t(1)}, _length{0}, _capacity{0} {}

    String(const String &s)
        : _cstr{s._cstr}, _used{s._used}, _length{s._length}, _capacity{s._capacity} {
          ++*_used;
        }

    String(const char *cstr) {
      if (cstr == NULL) 
        return;
      _initString(cstr, strlen(cstr));
    }

    ~String() {
      _decUsed();
    }

    String& operator=(const String& s) {
      // 拷贝赋值函数
      /*
          String u {"abcdefg"};
          String v {"hijknml"};
          v = u; // 必须要把 v 原来的字符串释放掉
      */
      ++*(s._used);
      _decUsed(); // 这两步可防止 s == *this 的情况
      _cstr = s._cstr;
      _length = s._length;
      _capacity = s._capacity;
      _used = s._used;
      return *this;
    }

    friend ostream& operator<<(ostream& os, String& s);
    char& operator[] (size_t i) {
      // 可能发生写操作必要调用深度复制
      _unalias();
      // 即使超过也返回最后一个字符
      if (i >= _length)
        return _cstr[_length - 1];
      return _cstr[i];
    }

    bool operator==(const String& other) {
      if (_cstr == other._cstr)
        return true;
      if (_length != other._length) {
        return false;
      } else {
        return std::strncmp(_cstr, other._cstr, _length) == 0;
      }
    }

 private:
    void _initString(const char *cstr, size_t len) {
      if (cstr == NULL) 
        return;
      _cstr = new char[len + 1];
      memcpy(_cstr, cstr, len);
      _cstr[len] = 0;
      _used = new size_t{1};
      _length = len;
      _capacity = len;
    }

    void _decUsed() {
      --*_used;
      if (*_used == 0) {
        if (_cstr != NULL) {
          cout << "delete: " << _cstr << endl;
          delete[] _cstr;
          _cstr = NULL;
          _length = 0;
          _capacity = 0;
        }

        delete _used;
        _used = NULL;
      }
    }

    void _unalias() {
      // 在写数据前必须复制一份(*_used > 1时)
      if (*_used == 1)
        return;
      _decUsed();
      _initString(_cstr, _length);
    }


 private:
    char *_cstr;
    size_t *_used;    // _used 为什么设置为指针? 为了保持使用同步!
    size_t _length;
    size_t _capacity;
};

ostream& operator<<(ostream&, String& s) {
  return cout << "[" << s._cstr << "] _used: " << *(s._used);
}

TEST_F(Copy_On_Write, copy_on_write_test) {
  String s{"1234567890"};
  // 隐式共享, 浅拷贝, 写时深拷贝
  String t = s;
  s[1] = '-';
  t[1] = '.'; // 写操作
  cout << s << endl;
  cout << t << endl;
  ASSERT_FALSE(s==t);

  String u {"abcdefg"};
  String v {"hijknml"};
  v = u;  // 这里会释放 v 的字符串
  cout << u << endl;
  cout << v << endl;
  ASSERT_TRUE(u==v);
}
