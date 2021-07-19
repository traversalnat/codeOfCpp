#include <gmock/gmock-matchers.h>
#include <gtest/gtest-death-test.h>
#include <gtest/gtest-param-test.h>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>
#include <sys/_types/_u_int64_t.h>
#include <sys/_types/_uintptr_t.h>

#include <iostream>
#include <string>
#include <vector>

using namespace std;
using testing::Test;
class Plugin : public Test {};

#define SLOT(a) #a
#define OFFSETOF_MEMBER(t, f) \
  (reinterpret_cast<uintptr_t>(&reinterpret_cast<t*>(16)->f) - static_cast<uintptr_t>(16u)) // NOLINT

#define OFFSET_MEMBER(offset) \
	(reinterpret_cast<uint64_t>(*(void **) (&offset)))

typedef void (*func) (void);

class OSGi {
 public:
    static OSGi* _instance;
    static OSGi* instance() {
        if (!_instance) {
            _instance = new OSGi();
        }
        return _instance;
    }

    // static void run(const void* classz, const uintptr_t func_offset) {
	//     uintptr_t _func = reinterpret_cast<uintptr_t>(classz) + func_offset;
	//     func new_func = (func) _func;
	//     new_func();
    // }
	static void run(const void* classz, uint64_t offset) {
		
	}

 private:
    OSGi() {}
};

class Worker {
 public:
    Worker() {
        this->str = "hello";
		// OSGi::run(this, OFFSET_MEMBER(&Worker::func));
		typedef void (Worker::*worker_func) ();
		worker_func offset_func = &Worker::func;
		cout << (reinterpret_cast<uint64_t>(*(void **) (&offset_func))) << endl;
    }
    void func() { std::cout << str << std::endl; }

 private:
    std::string str;
};

TEST_F(Plugin, plugin_test) { Worker worker; }
