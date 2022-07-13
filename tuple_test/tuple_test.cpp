#include <gmock/gmock-matchers.h>
#include <gtest/gtest-death-test.h>
#include <gtest/gtest-param-test.h>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>

#include <cstddef>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

using testing::Test;
class Tuple_Test : public Test {};

template <typename T>
struct Identity_ {
    using type = T;
};

template <size_t Pos, typename... TTags>
struct Pos2Type_ {
    static_assert((Pos != 0), "Invalid position.");
};

template <size_t Pos, typename TCur, typename... TTags>
struct Pos2Type_<Pos, TCur, TTags...> {
    using type =
        typename std::conditional<(Pos == 0), Identity_<TCur>,
                                  Pos2Type_<Pos - 1, TTags...>>::type::type;
};

// 得到 TTags... 中 Pos 位置的类型
template <size_t Pos, typename... TTags>
using Pos2Type = typename Pos2Type_<Pos, TTags...>::type;

template <class... Values>
class Tuple;

template <>
class Tuple<> {};

template <class Head, class... Tail>
class Tuple<Head, Tail...> : private Tuple<Tail...> {
 public:
    using inherited = Tuple<Tail...>;

    Tuple() {}
    Tuple(Head h, Tail... vtail) : m_head{h}, inherited(vtail...) {}
    Head head() { return m_head; }
    inherited& tail() { return *this; }

    // LIBCPP >= C++ 17
    // template <size_t N>
    // Pos2Type<N, Head, Tail...>& get() {
    //     if constexpr (N == 0)
    //         return m_head;
    //     else
    //         return tail().template get<N-1>();
    // }

    // LIBCPP < C++ 17
    // 使用 SFINAE (匹配失败非错误), N == 0 直接返回 m_head, N != 0 时,
    // 使用tail().template get<N-1>() 构建循环
    template <size_t N,
              typename std::enable_if<(N == 0), void>::type* = nullptr>
    Pos2Type<N, Head, Tail...>& get() {
        return m_head;
    }

    template <size_t N,
              typename std::enable_if<!(N == 0), void>::type* = nullptr>
    Pos2Type<N, Head, Tail...>& get() {
        return tail().template get<N - 1>();
    }

 private:
    Head m_head;
};

TEST_F(Tuple_Test, tuple_test_test) {
    auto tp = Tuple<int, char, float, char>{1, 'a', 1.2, 'b'};
    std::cout << tp.get<2>() << std::endl;
}
