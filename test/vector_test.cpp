#include "vector.h"
#include <gtest/gtest.h>

namespace
{
    struct CopyOnly
    {
        CopyOnly() = default;
        CopyOnly(int a) : m_a(a) {}
        CopyOnly(const CopyOnly&) = default;
        CopyOnly& operator=(const CopyOnly&) = default;
        CopyOnly(CopyOnly&&) = delete;
        CopyOnly& operator=(CopyOnly&&) = delete;
        int m_a;
    };

    struct MoveOnly
    {
        MoveOnly() = default;
        MoveOnly(int a) : m_a(a) {}
        MoveOnly(const MoveOnly&) = delete;
        MoveOnly& operator=(const MoveOnly&) = delete;
        MoveOnly(MoveOnly&&) = default;
        MoveOnly& operator=(MoveOnly&&) = default;
        int m_a;
    };
}   // anonymous namespace



TEST(VectorTest, IncludeTest) 
{
    my::vector<int> vec;
}

TEST(VectorTest, PushBackTest) 
{
    my::vector<CopyOnly> vecCopy;
    CopyOnly c;
    vecCopy.push_back(c);
    EXPECT_EQ(vecCopy.size(), 1);
    
    my::vector<MoveOnly> vecMove;
    vecMove.push_back(MoveOnly());
    EXPECT_EQ(vecMove.size(), 1);
}

TEST(VectorTest, EmplaceBackTest) 
{
    my::vector<CopyOnly> vecCopy;
    vecCopy.emplace_back(10);
    EXPECT_EQ(vecCopy.size(), 1);
    
    my::vector<MoveOnly> vecMove;
    vecMove.emplace_back(10);
    EXPECT_EQ(vecMove.size(), 1);
}

TEST(VectorTest, PopBackTest) 
{
    my::vector<CopyOnly> vecCopy;
    my::vector<MoveOnly> vecMove;
    for (int i = 0; i < 10; i++)
    {
        vecCopy.emplace_back(10);
        vecMove.emplace_back(10);
    }
    vecCopy.pop_back();
    EXPECT_EQ(vecCopy.size(), 9);   
    vecMove.pop_back();      
    EXPECT_EQ(vecMove.size(), 9);
}

TEST(VectorTest, ReserveTest) 
{
    my::vector<CopyOnly> vecCopy;
    my::vector<MoveOnly> vecMove;
    vecCopy.reserve(10);
    EXPECT_EQ(vecCopy.capacity(), 10);
    vecMove.reserve(10);
    EXPECT_EQ(vecMove.capacity(), 10);
    for (int i = 0; i < 20; i++)
    {
        vecCopy.emplace_back(10);
        vecMove.emplace_back(10);
    }
    EXPECT_EQ(vecCopy.size(), 20);
    EXPECT_EQ(vecMove.size(), 20);
    // reserve new_cap of 10 < 20 created elements, capacity should not change
    size_t vecCopyCap = vecCopy.capacity();
    vecCopy.reserve(10);
    EXPECT_EQ(vecCopy.capacity(), vecCopyCap);
    size_t vecMoveCap = vecMove.capacity();
    vecMove.reserve(10);
    EXPECT_EQ(vecMove.capacity(), vecMoveCap);
}

TEST(VectorTest, ReallocTest) 
{
    my::vector<CopyOnly> vecCopy;
    my::vector<MoveOnly> vecMove;
    for (int i = 0; i < 10000000; i++)
    {
        vecCopy.emplace_back(10);
        vecMove.emplace_back(10);
    }
    EXPECT_EQ(vecCopy.size(), 10000000);
    EXPECT_EQ(vecMove.size(), 10000000);
    vecCopy.clear();
    EXPECT_EQ(vecCopy.size(), 0);
    vecMove.clear();
    EXPECT_EQ(vecMove.size(), 0);
}

TEST(VectorTest, CopyConstructorTest) 
{
    my::vector<CopyOnly> vecCopy1;
    for (int i = 0; i < 10000000; i++)
        vecCopy1.emplace_back(10);
    EXPECT_EQ(vecCopy1.size(), 10000000);
    // copy constructor
    my::vector<CopyOnly> vecCopy2(vecCopy1);
    EXPECT_EQ(vecCopy2.size(), 10000000);
    EXPECT_EQ(vecCopy2[100].m_a, 10);
    EXPECT_EQ(vecCopy2.capacity(), vecCopy1.capacity());
}

TEST(VectorTest, MoveConstructorTest) 
{
    my::vector<CopyOnly> vecCopy1;
    for (int i = 0; i < 10000000; i++)
        vecCopy1.emplace_back(10);
    EXPECT_EQ(vecCopy1.size(), 10000000);
    // move constructor
    my::vector<CopyOnly> vecCopy2(std::move(vecCopy1));
    EXPECT_EQ(vecCopy2.size(), 10000000);
    EXPECT_EQ(vecCopy2[100].m_a, 10);
    EXPECT_EQ(vecCopy1.capacity(), 0);
    EXPECT_EQ(vecCopy1.size(), 0);
    vecCopy1.~vector<CopyOnly>();   // test destructor after move
    EXPECT_EQ(vecCopy1.capacity(), 0);
    EXPECT_EQ(vecCopy1.size(), 0);

    my::vector<MoveOnly> vecMove1;
    for (int i = 0; i < 10000000; i++)
        vecMove1.emplace_back(10);
    EXPECT_EQ(vecMove1.size(), 10000000);
    // move constructor
    my::vector<MoveOnly> vecMove2(std::move(vecMove1));
    EXPECT_EQ(vecMove2.size(), 10000000);
    EXPECT_EQ(vecMove2[100].m_a, 10);
    EXPECT_EQ(vecMove1.capacity(), 0);
    EXPECT_EQ(vecMove1.size(), 0);
    vecMove1.~vector<MoveOnly>();   // test destructor after move
    EXPECT_EQ(vecMove1.capacity(), 0);
    EXPECT_EQ(vecMove1.size(), 0);

}
