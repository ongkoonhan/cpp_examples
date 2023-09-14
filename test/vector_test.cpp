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

TEST(VectorTest, ReallocTest) 
{
    my::vector<CopyOnly> vecCopy;
    my::vector<MoveOnly> vecMove;
    for (int i = 0; i < 1000; i++)
    {
        vecCopy.emplace_back(10);
        vecMove.emplace_back(10);
    }
    EXPECT_EQ(vecCopy.size(), 1000);
    EXPECT_EQ(vecMove.size(), 1000);
    vecCopy.clear();
    EXPECT_EQ(vecCopy.size(), 0);
    vecMove.clear();
    EXPECT_EQ(vecMove.size(), 0);
}