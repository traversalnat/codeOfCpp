#include <cstdio>
#include <iostream>

// shared_ptr 的实现思路
template <typename T>
class s_ptr {
 public:
    s_ptr() {}
    ~s_ptr() {
        if (--*puc) delete p;
    }

    T get() const { return *p; }

    void set(const T& value) { *p = value; }

 private:
    T* p;
    std::atomic<int>* puc;
};

int fct(int v) {
	return v + 1;
}

int main() {
	std::atomic<int> val {0};
	int expected = val.load();
	int next;
	do {
		next = fct(expected);
		std::cout << expected << " next: " << next;
	} while (!val.compare_exchange_weak(expected, next));
}
