#include <gtest/gtest.h>
#include <vector>
#include <cassert>
#include <algorithm>
#include <random>
#include "scoped_timer.h"

template <typename Iterator>
auto contains_duplicates_n2(Iterator begin, Iterator end) {
  ScopedTimer s("contains_duplicates_n2");
  for (auto it = begin; it != end; ++it)
    if (std::find(std::next(it), end, *it) != end)
      return true;
  return false;
}

template <typename Iterator>
auto contains_duplicates_allocating(Iterator first, Iterator last) {
  ScopedTimer s("contains_duplicates_allocating");
  // As (*begin) returns a reference, we have to get the base type using std::decay_t
  using ValueType = std::decay_t<decltype(*first)>;
  auto copy = std::vector<ValueType>(first, last);
  std::sort(copy.begin(), copy.end());
  return std::adjacent_find(copy.begin(), copy.end()) != copy.end();
}

TEST(ContainsDuplicates, Examples) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(1000, 1999);
  
  auto vals = std::vector<int>(100000);
  std::ranges::generate(vals, [&]() { return dis(gen); });

  auto a = contains_duplicates_n2(vals.begin(), vals.end());
  ASSERT_TRUE(a);

  auto b = contains_duplicates_allocating(vals.begin(), vals.end());
  ASSERT_TRUE(b);
}
