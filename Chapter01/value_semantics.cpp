#include <cassert>
#include <gtest/gtest.h>
#include <set>
#include <string>

//
// This example demonstrates the use of value semantics.
// The set of toppings, t, is passed by value and
// completely copied and encapsulated by the Bagel class.
//

using MySet = std::set<std::string, std::less<>>;

class Bagel {
public:
  Bagel(MySet ts) : toppings_(std::move(ts)) {}

private:
  MySet toppings_;
};

TEST(ValueSemantics, CreateBagels) {

  auto t = MySet{};
  t.insert("salt");
  auto a = Bagel{t};

  // ‘a’ is not affected
  // when adding pepper
  t.insert("pepper");

  // ‘a’ will have salt
  // ‘b’ will have salt & pepper
  auto b = Bagel{t};

  // No bagel is affected
  t.insert("oregano");
}
