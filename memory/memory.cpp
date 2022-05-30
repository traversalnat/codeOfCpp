#include <malloc/_malloc.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class vec {
 public:
    vec(string name_t) : name{name_t} {
        std::cout << "create " << name << std::endl;
    }

    ~vec() { std::cout << "destroy " << name << std::endl; }

    void print() {
        std::cout << name << ":";
        printf("mem %s\n", mem);
    }

    void modify() {
        mem = new char[name.size()];
        memcpy(mem, name.c_str(), name.size());
    }

    void more_modify() {
        mem[0] = 'N';
    }

 private:
    string name;
    char *mem;
};

template <class T1, class T2>
void construct(T1 *p, T2 &value) {
    new (p) T1(value);
}

int main(int argc, char *argv[]) {
    vec v1{"v1_hello"};
    v1.modify();
    vec v2{"v2_hello"};
    v2.modify();
    vec v3{"v3_hello"};
    v3.modify();
    vec *v = (vec *)malloc(sizeof(vec));
    construct(v, v1);
    v->print();
    construct(v, v2);
    v->print();
    construct(v, v3);
    // 测试浅拷贝, v3 mem 修改后, v 的 mem 也跟着修改
    v3.more_modify();
    v->print();

    delete v;
    return 0;
}
