#include "shared_ptr.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>


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

// Destructor Test

namespace
{
class mock_control_block
{
public:
    using Ctrl = my::__control_block;
    // wrapper functions
    void increment_shared() { ctrl.increment_shared(); }
    void decrement_shared() { ctrl.decrement_shared(); }
    size_t shareds() const noexcept { return ctrl.shareds(); }
    size_t refs() const noexcept { return ctrl.refs(); } 
    // mock test functions
    ~mock_control_block() 
    { 
        ctrl.~__control_block();
        Die();
    }
    MOCK_METHOD(void, Die, (), (const));   // track destructor call
private:
    Ctrl ctrl;
};

template <typename T, typename Ctrl = mock_control_block>
class mock_shared_ptr : public my::__shared_ptr<T, Ctrl>
{
public:
    mock_shared_ptr() : my::__shared_ptr<T, Ctrl>() { }
    explicit mock_shared_ptr(T* ptr) : my::__shared_ptr<T, Ctrl>(ptr) { }
    mock_shared_ptr(const mock_shared_ptr<T>& other) noexcept : my::__shared_ptr<T, Ctrl>(other) { }
    // for mock unit testing
    const Ctrl* get_control_block() const noexcept
    {
        return my::__shared_ptr<T, Ctrl>::get_control_block();
    }
};
}   // anonymous namespace

TEST(SharedPtrTest, DestructorTest) 
{
    mock_shared_ptr<int> ptr(new int(10));
    mock_shared_ptr<int> ptr2(ptr);
    EXPECT_EQ(ptr.use_count(), 2);
    // test control block destruction when shared_ptr is out of scope
    const mock_control_block* ctrl = ptr.get_control_block();
    EXPECT_NE(ctrl, nullptr);
    EXPECT_CALL(*ctrl, Die());
}