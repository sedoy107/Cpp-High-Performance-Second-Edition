#include <set>
#include <string>
#include <cassert>

#define USE_GTEST 0

#if USE_GTEST == 1
#include <gtest/gtest.h>
#endif

int my_main(int argc, char **argv);
int main(int argc, char **argv) {

#if USE_GTEST == 1
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
#else
  return my_main(argc, argv);
#endif
}

using MySet = std::set<std::string, std::less<>>;
class Bagel {
public:
  Bagel(MySet ts) : toppings_(std::move(ts)) {}
  // Bagel(MySet& ts) : toppings_(std::move(ts)) {} // will move from original ts

private:
  MySet toppings_;
};

void value_semantics_move_bagel() {

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

int my_main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {

  value_semantics_move_bagel();

  return 0;
}
