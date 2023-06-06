#include <gtest/gtest.h>

auto pow_n(const auto& v, int n) {
  auto product = decltype(v){1}; // Instead of T{1}
  // or:
  // std::remove_cvref<decltype(v)>::type product{1};
  for (int i = 0; i < n; ++i) {
    product *= v;
  }
  return product;
}

TEST(AbbreviatedFunctionTemplates, PowN) {
  auto x = pow_n(3, 3); // x is an int

  ASSERT_EQ(27, x);
  ASSERT_EQ(typeid(int), typeid(x));
}

TEST(AbbreviatedFunctionTemplates, ExplicitTemplateParameters) {

  auto pow_n_func = []<class T>(const T& v, int n) {
    auto product = T{1};
    for (int i = 0; i < n; ++i) {
      product *= v;
    }
    return product;
  };

  auto x = pow_n_func(3, 3); // x is an int
  ASSERT_EQ(27, x);
  ASSERT_EQ(typeid(int), typeid(x));
}