#ifndef POINTER_TO_MEMFUNC_H
#define POINTER_TO_MEMFUNC_H
#include <any>
#include <functional>

template <typename T, typename... Args>
auto tuple_append(T &&t, Args &&...args) {
    return std::tuple_cat(std::forward<T>(t), std::forward_as_tuple(args...));
}

/**
 * @brief mybindfront: c++20 bind_front 的替代实现
 *
 * @tparam F
 * @tparam FrontArgs
 * @param
 * @param ...frontArgs
 */
template <typename F, typename... FrontArgs>
decltype(auto) mybindfront(F &&f, FrontArgs &&...frontArgs) {
    return [f = std::forward<F>(f),
            frontArgs = std::make_tuple(std::forward<FrontArgs>(frontArgs)...)](
               auto &&...backArgs) {
        return std::apply(
            f, tuple_append(frontArgs,
                            std::forward<decltype(backArgs)>(backArgs)...));
    };
}

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

// PUBLIC_SIGNAL 为信号函数模板代码, 可以设置成宏
// auto anyslot_vec = slot_map[typeid(&Sender::signal).name()];
// for (auto anyslot : anyslot_vec) {
//     try {
//         auto slot = any_cast<
//             function<bare_func<decltype(&Sender::signal)>::type>>(
//             anyslot);
//         slot(str);
//     } catch (std::bad_cast e) {
//         cout << "Bad slot: " << anyslot.type().name() << endl;
//         return;
//     }
// }

#define PUBLIC_SIGNAL(Object, Signal, ...)                                 \
    do {                                                                   \
        auto anyslot_vec = slot_map[typeid(&Object::Signal).name()];       \
                                                                           \
        for (auto anyslot : anyslot_vec) {                                 \
            try {                                                          \
                auto slot = std::any_cast<                                 \
                    function<bare_func<decltype(&Object::Signal)>::type>>( \
                    anyslot);                                              \
                slot(__VA_ARGS__);                                         \
            } catch (std::bad_cast e) {                                    \
                cout << "Bad slot: " << anyslot.type().name() << endl;     \
                return;                                                    \
            }                                                              \
        }                                                                  \
    } while (0);

#endif  // POINTER_TO_MEMFUNC_H
