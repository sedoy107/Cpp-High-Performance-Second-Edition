#include <gtest/gtest.h>
#include <boost/contract.hpp>
#include <iostream>

void inc(int& x) {
    boost::contract::old_ptr<int> old_x = BOOST_CONTRACT_OLDOF(x); // save old value
    boost::contract::check c = boost::contract::function()
        .precondition([&] {
            BOOST_CONTRACT_ASSERT(x < std::numeric_limits<int>::max());
        })
        .postcondition([&] {
            BOOST_CONTRACT_ASSERT(x == *old_x + 1);
        })
    ;

    ++x; // Function body.
}

int divide(int dividend, int divisor) {
    int result;
    boost::contract::check c = boost::contract::function()
        .precondition([&] {
            BOOST_CONTRACT_ASSERT(divisor != 0);
        })
        .postcondition([&] {
            BOOST_CONTRACT_ASSERT(result * divisor == dividend);
        })
    ;

    return result = dividend / divisor; // Function body.
}

TEST(Boost_Contract, Increment) {
    int x = 0;
    inc(x);
    ASSERT_EQ(x, 1);
    inc(x);
    ASSERT_EQ(x, 2);
}

TEST(Boost_Contract, Division) {
    ASSERT_EQ(divide(10, 2), 5);
    //ASSERT_THROW(divide(10, 0), boost::contract::assertion_failure); // not propagated to this point
}