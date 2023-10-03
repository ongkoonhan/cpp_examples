#include "shared_ptr.h"
#include <gtest/gtest.h>


TEST(SharedPtrTest, IncludeTest) 
{
    my::shared_ptr<int> ptr;
}

TEST(SharedPtrTest, CopyTest) 
{
    my::shared_ptr<int> ptr(new int(10));
    my::shared_ptr<int> ptr2(ptr);
    EXPECT_EQ(ptr.use_count(), 2);
    EXPECT_EQ(ptr2.use_count(), 2);
}

TEST(SharedPtrTest, MoveTest) 
{
    my::shared_ptr<int> ptr(new int(10));
    my::shared_ptr<int> ptr2(std::move(ptr));
    EXPECT_EQ(ptr.use_count(), 0);
    EXPECT_EQ(ptr2.use_count(), 1);
}

TEST(SharedPtrTest, ResetTest_1) 
{
    my::shared_ptr<int> ptr(new int(10));
    my::shared_ptr<int> ptr2 = ptr;
    EXPECT_EQ(ptr.use_count(), 2);
    EXPECT_EQ(ptr2.use_count(), 2);
    ptr.reset();
    EXPECT_EQ(ptr.use_count(), 0);
    EXPECT_EQ(ptr2.use_count(), 1);
    EXPECT_EQ(ptr.get(), nullptr);
    EXPECT_EQ(*ptr2, 10);
}

TEST(SharedPtrTest, ResetTest_2) 
{
    my::shared_ptr<int> ptr(new int(10));
    my::shared_ptr<int> ptr2 = ptr;
    EXPECT_EQ(ptr.use_count(), 2);
    EXPECT_EQ(ptr2.use_count(), 2);
    ptr.reset(new int(20));
    EXPECT_EQ(ptr.use_count(), 1);
    EXPECT_EQ(ptr2.use_count(), 1);
    EXPECT_EQ(*ptr, 20);
    EXPECT_EQ(*ptr2, 10);
}

// TEST(SharedPtrTest, DestructorTest) 
// {
//     my::shared_ptr<int> ptr;
//     {
//         // requires reset/swap
//     }
// }