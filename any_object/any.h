#ifndef _ANY_H
#define _ANY_H
#include <typeinfo>
using std::type_info;

namespace hlp {
class TypeRemover {
 public:
    virtual ~TypeRemover() {}
};

template <typename T>
class DataHolder : public TypeRemover {
 public:
    DataHolder(const T& data) : _data{data} {}
    ~DataHolder() {}
    T _data;
};

class any {
 public:
    any() : _content(nullptr), _h{nullptr}, _used{new size_t{1}} {}

    /**
     * @brief 拷贝 value 至 any 容器中
     *
     * @tparam T
     * @param value
     */
    template <typename T>
    any(const T& value)
        : _content{new DataHolder<T>(value)},
          _h{nullptr},
          _used{new size_t{1}} {
        _h = []() {
            return const_cast<void*>(static_cast<void const*>(&typeid(T)));
        };
    }

    any(const any& rhs) : _content{rhs._content}, _h{rhs._h}, _used{rhs._used} {
        ++*_used;
    }

    ~any() { _decUsed(); }

    any& swap(any& rhs) {
        std::swap(_content, rhs._content);
        std::swap(_h, rhs._h);
        std::swap(_used, rhs._used);
        return *this;
    }

    template <typename T>
    any& operator=(const T& rhs) {
        any(rhs).swap(*this);
        return *this;
    }

    any& operator=(const any& rhs) {
        any(rhs).swap(*this);
        return *this;
    }

    /**
     * @brief 返回 _content 类型信息
     *
     * @return
     */
    const type_info& type() const {
        return *static_cast<type_info const*>(_h());
    }

    template <typename U>
    friend U& any_cast(const any& rhs);

 private:
    /**
     * @brief 擦除类型后的数据
     */
    TypeRemover* _content;
    /**
     * @brief _h 函数保存 _content 的类型信息
     *
     * @param none
     *
     * @return void * 形式存在的 _content 的 typeinfo
     */
    void* (*_h)();
    /**
     * @brief _content 引用计数
     */
    size_t* _used;

 private:
    /**
     * @brief 减少引用计数
     */
    void _decUsed() {
        if (--(*_used) == 0) {
            delete _content;
            delete _used;
            _content = nullptr;
            _used = nullptr;
        }
    }
};

/**
 * @brief 将 hlp::any 类型数据转换为 T 类型的数据
 *
 * @tparam T
 * @param rhs
 *
 * @throw std::bad_cast any 保存数据的类型与 T 不符时
 *
 * @return
 */
template <typename T>
T& any_cast(const any& rhs) {
    if (typeid(T) == rhs.type()) {
        return static_cast<DataHolder<T>*>(rhs._content)->_data;
    }
    throw std::bad_cast{};
}

}  // namespace hlp

#endif
