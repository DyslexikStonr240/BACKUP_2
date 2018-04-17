#include "../googletest/googletest/include/gtest/gtest.h"
#include <iostream>

#include "../../src/include/List.h"


TEST(ListTest, create_empty_list){
    List<int> list;
    EXPECT_EQ(list.size(), 0);
    EXPECT_EQ(list.is_init(), false);
}

TEST(ListTest, read_from_beyond_range){
    int size = 5;
    List<int> list(size, 0);
    ASSERT_DEATH({list.at(size + 1);}, "Cannot read element beyond the end of a list");
}
TEST(ListTest, create_list_of_size){
    int size = 5;
    List<int> list(size, 0);
    EXPECT_EQ(list.size(), size);
    EXPECT_EQ(list.is_init(), true);
    EXPECT_EQ(list.at(size - 1), 0);
}
TEST(ListTest, insert_back){
    int size = 5;
    List<int> list(size, 0);
    ASSERT_DEATH({list.insert_back(1, size + 1);}, "Cannot insert element beyond the end of a list");
    list.insert_back(1, size - 1);
    EXPECT_EQ(list.size(), size + 1);
    EXPECT_EQ(list.at(size), 1);
}
TEST(ListTest, insert_front){
    int size = 5;
    List<int> list(size, 0);
    //ASSERT_DEATH({list.insert_back(1, size + 1);}, "Cannot insert element beyond the end of a list");
    list.insert_front(1, size - 1);
    // EXPECT_EQ(list.size(), size + 1);
    // EXPECT_EQ(list.at(size), 1);
}
TEST(ListTest, remove_at){
    int size = 5;
    List<int> list(size, 0);
    ASSERT_DEATH({list.remove_at(size);}, "Cannot remove element beyond the end of a list");
    // list.remove_back(size - 1);
    // EXPECT_EQ(list.size(), size - 1);
}
TEST(ListTest,push_back){
    int size = 0;
    List<int> list(size, 0);
    list.push_back(0);
    EXPECT_EQ(list.size(), size + 1);
    EXPECT_EQ(list.at(size), 0);
}

TEST(ListTest, pop_back){
    int size = 0;
    List<int> list(size, 0);
    //list.pop_back();
    ASSERT_DEATH({list.pop_back();}, "Cannot remove element from an empty list");
    list.push_back(0);
    list.pop_back();
    EXPECT_EQ(list.size(), 0);
}
