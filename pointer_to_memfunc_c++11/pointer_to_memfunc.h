#ifndef POINTER_TO_MEMFUNC_H
#define POINTER_TO_MEMFUNC_H
#include <functional>
#include <iostream>
/**
 * @brief 获取成员函数指针S(T::*)(Args...) 的普通函数类型 S()(Args...)
 * 如传入类型为 void (Receiver::*)(string) 得到的 type 为 void()(string)
 *
 * @brief std::function<typename bare_func<decltype(&Receiver::slot)>::type>
 * 注: typename 是必要的, 因为 type 是一个类型, 而不是一个类型名
 * 注: decltype 可以用于找到 &Receiver::slot 的实际类型
 *
 * @tparam S
 * @tparam T
 * @tparam Args
 */
template <typename T>
struct bare_func;

template <typename S, typename T, typename... Args>
struct bare_func<S (T::*)(Args...)> {
    typedef S(type)(Args...);
};

template <typename T>
using bare_func_type = typename bare_func<T>::type;

// PUBLIC_SIGNAL 为信号函数模板代码, 可以设置成宏

#define PUBLIC_SIGNAL(Object, Signal, ...)                                 \
    do {                                                                   \
        auto anyslot_vec = slot_map[typeid(&Object::Signal).name()];       \
                                                                           \
        for (auto anyslot : anyslot_vec) {                                 \
            try {                                                          \
                auto slot = any_cast<                                      \
                    function<bare_func_type<decltype(&Object::Signal)>>>( \
                    anyslot);                                              \
                slot(__VA_ARGS__);                                         \
            } catch (std::bad_cast e) {                                    \
                cout << "Bad slot: " << anyslot.type().name() << endl;     \
                return;                                                    \
            }                                                              \
        }                                                                  \
    } while (0);

/**
 * @brief 将成员函数与类的实例绑定得到一个可以像普通函数一样调用的可调用对象
 * 可转换成 function<> 类型, 实际上 function<> 是对可调用对象的一层包装
 * @tparam R    类: string
 * @tparam SLOT 成员函数, &string::append
 * string 
 */
template <typename R, typename SLOT>
struct _binded_mem_fn {
    R *r;
    SLOT slot;

    _binded_mem_fn(R *r, SLOT slot) : r{r}, slot{slot} {}

    template <typename... Args>
    void operator()(Args&&... args) {
        auto f = std::mem_fn(slot);
        f(r, std::forward<Args>(args)...);
    }
};

/**
 * @brief 
 *
 * @tparam R
 * @tparam SLOT
 * @param r
 * @param slot
 *
 * @return 可调用对象 _binded_mem_fn<R, SLOT>
 */
template <typename R, typename SLOT>
_binded_mem_fn<R, SLOT> binded_mem_fn(R *r, SLOT slot) {
    return _binded_mem_fn<R, SLOT>(r, slot);
}

#endif  // POINTER_TO_MEMFUNC_H
