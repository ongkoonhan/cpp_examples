#include "vector.h"
#include <gtest/gtest.h>

namespace
{
    struct CopyOnly
    {
        CopyOnly() = default;
        CopyOnly(const CopyOnly&) = default;
        CopyOnly& operator=(const CopyOnly&) = default;
        CopyOnly(CopyOnly&&) = delete;
        CopyOnly& operator=(CopyOnly&&) = delete;
        int a;
    };

    struct MoveOnly
    {
        MoveOnly() = default;
        MoveOnly(const MoveOnly&) = delete;
        MoveOnly& operator=(const MoveOnly&) = delete;
        MoveOnly(MoveOnly&&) = default;
        MoveOnly& operator=(MoveOnly&&) = default;
        int a;
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
