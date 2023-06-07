#include <set>
#include <string>
#include <cassert>
#include <vector>
#include <algorithm>
#include <iostream>
#include <random>
#include <ranges>
#include <format>
#include <concepts>
#include <type_traits>
#include <numeric>
#include <thread>
#include <memory>


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

struct NonMovable {
  NonMovable() = default;
  NonMovable(const NonMovable&) = default;
  NonMovable(NonMovable&&) = delete;
  NonMovable& operator=(const NonMovable&) = default;
  NonMovable& operator=(NonMovable&&) = delete;
  private:
    int x_ = 42;
};

using MySet = std::set<std::string, std::less<>>;
class Bagel {
public:
  Bagel(MySet ts, NonMovable nm) : toppings_(std::move(ts)), non_movable_(std::move_if_noexcept(nm)) {}
  // Bagel(MySet& ts) : toppings_(std::move(ts)) {} // will move from original ts

private:
  MySet toppings_;
  NonMovable non_movable_;
};

void value_semantics_move_bagel() {

  auto t = MySet{};
  t.insert("salt");
  auto a = Bagel{t, NonMovable{}};

  // ‘a’ is not affected
  // when adding pepper
  t.insert("pepper");

  // ‘a’ will have salt
  // ‘b’ will have salt & pepper
  auto b = Bagel{t, NonMovable{}};

  // No bagel is affected
  t.insert("oregano");
}

void reference_binding_example(const std::string& a, const std::string& b) {
  const auto& temp_cstr = a + b;
  //auto& temp_str = a + b; // a non-const reference cannot be bound to a temporary
}

void string_view_example(std::string_view sv) {
  const std::string s{sv}; // copy
  const std::string_view sv2 = s; // view
  const char* cstr = sv.data(); // pointer to the string data
}

void ranges_transform_example() {

  auto square = [](int x) { return x * x; };
  auto v = std::vector{1, 2, 3, 4};

  // Insert into std::set
  auto squared_set = std::set<int>{};
  std::ranges::transform(v, std::inserter(squared_set, squared_set.end()), square);

  // Insert into std::vector
  auto squared_vec = std::vector<int>{};
  std::ranges::transform(v, std::back_inserter(squared_vec), square);
}

template <typename Container>
auto move_n_elements_to_back(Container& c, size_t n) {
  assert(n <= c.size());
  auto new_begin = std::next(c.begin(), n);
  return std::rotate(c.begin(), new_begin, c.end());
}

auto mk_vec(size_t n) {
  std::random_device rd;
  std::mt19937 gen(rd());

  auto v = std::vector<int>(n);
  v.reserve(n);

  std::iota(v.begin(), v.end(), 0);
  std::ranges::shuffle(v, gen);

  return v;
}

void print_container(auto&& c) {
  std::cout << "[";
  for (const auto& e : c) {
    std::cout << e << ", ";
  }
  std::cout << "]\n";
}

template <typename T>
concept Arithmetic = std::is_arithmetic_v<T>;

template<Arithmetic T>
Arithmetic auto constrained_diff(T x1, T x2) {
  return x1 - x2;
}

template<Arithmetic T>
void print_args(T x) {
  std::cout << x;
}


template <std::size_t N>
struct MyAllocator
{
    char data[N];
    void* p;
    std::size_t sz;
    MyAllocator() : p(data), sz(N) {}
    template <typename T>
    T* aligned_alloc(std::size_t a = alignof(T))
    {
        if (std::align(a, sizeof(T), p, sz))
        {
            T* result = reinterpret_cast<T*>(p);
            p = (char*)p + sizeof(T);
            sz -= sizeof(T);
            return result;
        }
        return nullptr;
    }
};

void myallocator_example() {
  MyAllocator<64> a;
  std::cout << "allocated a.data at " << (void*)a.data
            << " (" << sizeof a.data << " bytes)\n";

  // allocate a char
  if (char* p = a.aligned_alloc<char>()) {
      *p = 'a';
      std::cout << "allocated a char at " << (void*)p << '\n';
  }

  // allocate an int
  if (int* p = a.aligned_alloc<int>()) {
      *p = 1;
      std::cout << "allocated an int at " << (void*)p << '\n';
  }

  // allocate an int, aligned at 32-byte boundary
  if (int* p = a.aligned_alloc<int>(32)) {
      *p = 2;
      std::cout << "allocated an int at " << (void*)p << " (32 byte alignment)\n";
  }
}

int my_main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {

  value_semantics_move_bagel();

  ranges_transform_example();

  auto v = std::vector{1, 2, 3, 4, 5, 6, 7, 8};
  move_n_elements_to_back(v, 3);

#ifndef __clang__
  auto v2 = mk_vec(10);
  print_container(
    v2
    | std::views::transform([](int x) { return x * x; })
    | std::views::filter([](int x) { return x % 2 == 0; })
    | std::views::transform([](int x) { return std::format("\"{}\"", x); })
  );

  auto csv = std::string{"10,11,12"};
  auto digits = csv
    | std::views::split(',')
    | std::views::join;
  for (auto x : digits) {std::cout << x;}
#endif

  auto v3 = std::vector{4, 2, 7, 1, 2, 6, 1, 5};
  auto first_half = v3 | std::views::take(v3.size() / 2);
  std::ranges::sort(first_half);

  assert(constrained_diff(10, 4) == 6);
  //print_args("4"); // won't compile

  myallocator_example();

  std::cout << "Hardware threads: " << std::thread::hardware_concurrency() << "\n";

  return 0;
}
