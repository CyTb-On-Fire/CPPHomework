#include "gtest/gtest.h"
#include "set_container.hpp"


TEST(setTest, iterator){
set<int> s;
s.insert(1);
s.insert(5);
s.insert(6);
s.insert(2);


auto test = s.begin();
EXPECT_EQ(*test, 1);
EXPECT_NO_THROW(test++);
EXPECT_EQ(*test, 2);
EXPECT_NO_THROW(test--);
EXPECT_EQ(*test, 1);

}


TEST(set_test, insert){

}

TEST(set_test, erase){

}

TEST(set_test, find){

}


TEST(set_test, timeComparation){

}