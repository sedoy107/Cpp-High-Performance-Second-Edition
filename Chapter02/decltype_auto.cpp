#include <cassert>
#include <gtest/gtest.h>
#include <initializer_list>



struct Foo {
    Foo() = default;
    Foo(int m) : _m{m} {}
    auto val() const { return _m; }
    auto& cref() const { return _m; }
    auto& mref() { return _m; }
    int _m{};
};

namespace plain_auto {
    auto val_wrapper(auto& f) {
        return f.val();
    }

    auto cref_wrapper(auto& f) {
        return f.cref();
    }

    auto mref_wrapper(auto& f) {
        return f.mref();
    }
}

namespace decltype_auto {
    decltype(auto) val_wrapper(auto& f) {
        return f.val();
    }

    decltype(auto) cref_wrapper(auto& f) {
        return f.cref();
    }

    decltype(auto) mref_wrapper(auto& f) {
        return f.mref();
    }
}

TEST(Decltype_With_Auto, decltype_auto) {
    auto f = Foo{42};

    // When plain auto is used, the return type is deduced to be int: reference-ness is lost.
    static_assert(std::is_same_v<decltype(plain_auto::val_wrapper(f)), int>, "Object type mismatch!");
    static_assert(std::is_same_v<decltype(plain_auto::cref_wrapper(f)), int>, "Object type mismatch!");
    static_assert(std::is_same_v<decltype(plain_auto::mref_wrapper(f)), int>, "Object type mismatch!");

    // When decltype(auto) is used, the return type is deduced to be int&: reference-ness is preserved.
    static_assert(std::is_same_v<decltype(decltype_auto::val_wrapper(f)), int>, "Object type mismatch!");
    static_assert(std::is_same_v<decltype(decltype_auto::cref_wrapper(f)), int const&>, "Object type mismatch!");
    static_assert(std::is_same_v<decltype(decltype_auto::mref_wrapper(f)), int&>, "Object type mismatch!");

}
