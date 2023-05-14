#include <gtest/gtest.h>

int main(int argc, char **argv) {

  for (int i = 0; i < 100; ++i) {
    std::cout << i << std::endl;
  }

  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
