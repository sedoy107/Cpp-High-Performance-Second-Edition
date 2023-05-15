#include <gtest/gtest.h>

extern "C" {
  void download_webpage(const char* url, void (*callback)(int, const char*)); 
}

TEST(LambdaTypes, ConvertToFunctionPointer) {
  // auto callback = +[](int status, const char* content) {
  //                ^^^^ the plus sign isn't required anymore?
  auto callback = [](int status, const char* content) { // C++20
    std::cout << "Status: " << status << std::endl;
    std::cout << "Content: " << content << std::endl;
  };

  download_webpage("https://www.packtpub.com/", callback);
}

// Requires C++20
TEST(LambdaTypes, StatelessLambdas) {
  auto x = [] {}; // A lambda without captures
  auto y = x;     // Assignable
  decltype(y) z;  // Default-constructible
  static_assert(std::is_same_v<decltype(x), decltype(y)>); // passes
  static_assert(std::is_same_v<decltype(x), decltype(z)>); // passes
}