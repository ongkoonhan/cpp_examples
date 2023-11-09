#include "shared_ptr.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>


// shared_ptr tests

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
    ptr2 = ptr;   // copy assignment
    EXPECT_EQ(ptr.use_count(), 2);
    EXPECT_EQ(ptr2.use_count(), 2);
    ptr2 = ptr2;   // self assignment
    EXPECT_EQ(ptr.use_count(), 2);
    EXPECT_EQ(ptr2.use_count(), 2);
}

TEST(SharedPtrTest, MoveTest) 
{
    my::shared_ptr<int> ptr(new int(10));
    my::shared_ptr<int> ptr2(std::move(ptr));
    EXPECT_EQ(ptr.use_count(), 0);
    EXPECT_EQ(ptr2.use_count(), 1);
    ptr = ptr2;   // copy assignment
    EXPECT_EQ(ptr.use_count(), 2);
    EXPECT_EQ(ptr2.use_count(), 2);
    ptr2 = std::move(ptr);   // move assignment
    EXPECT_EQ(ptr.use_count(), 0);
    EXPECT_EQ(ptr2.use_count(), 1);
    ptr2 = std::move(ptr2);   // self assignment
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

// weak_ptr tests

TEST(WeakPtrTest, IncludeTest) 
{
    my::weak_ptr<int> ptr;
}

TEST(WeakPtrTest, CopyTest) 
{
    my::shared_ptr<int> s_ptr(new int(10));
    my::weak_ptr<int> ptr(s_ptr);
    my::weak_ptr<int> ptr2(ptr);   // copy
    EXPECT_EQ(s_ptr.use_count(), 1);
    EXPECT_EQ(ptr.use_count(), 1);
    my::shared_ptr<int> s_ptr2(s_ptr);
    EXPECT_EQ(s_ptr.use_count(), 2);
    EXPECT_EQ(ptr.use_count(), 2);
    EXPECT_FALSE(ptr.expired());
    ptr2 = ptr;   // copy assignment
    EXPECT_EQ(ptr.use_count(), 2);
    EXPECT_EQ(ptr2.use_count(), 2);
    ptr2 = ptr2;   // self assignment
    EXPECT_EQ(ptr.use_count(), 2);
    EXPECT_EQ(ptr2.use_count(), 2);
}

TEST(WeakPtrTest, MoveTest) 
{
    my::shared_ptr<int> s_ptr(new int(10));
    my::weak_ptr<int> ptr(s_ptr);
    my::weak_ptr<int> ptr2(std::move(ptr));   // move
    EXPECT_EQ(s_ptr.use_count(), 1);
    EXPECT_EQ(ptr.use_count(), 0);
    EXPECT_EQ(ptr2.use_count(), 1);
    my::shared_ptr<int> s_ptr2(s_ptr);
    EXPECT_EQ(s_ptr.use_count(), 2);
    EXPECT_EQ(ptr.use_count(), 0);
    EXPECT_EQ(ptr2.use_count(), 2);
    EXPECT_TRUE(ptr.expired());
    EXPECT_FALSE(ptr2.expired());
    ptr = ptr2;   // copy assignment
    EXPECT_EQ(ptr.use_count(), 2);
    EXPECT_EQ(ptr2.use_count(), 2);
    ptr2 = std::move(ptr);   // move assignment
    EXPECT_EQ(ptr.use_count(), 0);
    EXPECT_EQ(ptr2.use_count(), 2);
    ptr2 = std::move(ptr2);   // self assignment
    EXPECT_EQ(ptr2.use_count(), 2);
}

TEST(WeakPtrTest, LockTest_1)
{
    my::weak_ptr<int> ptr;
    my::shared_ptr<int> s_ptr = ptr.lock();
    EXPECT_EQ(ptr.use_count(), 0);
    EXPECT_EQ(s_ptr.use_count(), 0);
    EXPECT_EQ(s_ptr.get(), nullptr);
}

TEST(WeakPtrTest, LockTest_2) 
{
    my::shared_ptr<int> s_ptr(new int(10));
    my::weak_ptr<int> ptr(s_ptr);
    my::shared_ptr<int> s_ptr2 = ptr.lock();
    EXPECT_EQ(s_ptr.use_count(), 2);
    EXPECT_EQ(ptr.use_count(), 2);
    EXPECT_EQ(*s_ptr2, 10);
}

TEST(WeakPtrTest, LockTest_3) 
{
    my::shared_ptr<int> s_ptr(new int(10));
    s_ptr.reset();
    my::weak_ptr<int> ptr(s_ptr);
    my::shared_ptr<int> s_ptr2 = ptr.lock();
    EXPECT_EQ(s_ptr.use_count(), 0);
    EXPECT_EQ(ptr.use_count(), 0);
    EXPECT_EQ(s_ptr2.get(), nullptr);
}

// Destructor Tests

namespace
{

class mock_control_block
{
public:
    using Ctrl = my::__control_block;
    // wrapper functions
    void increment_shared() { ctrl.increment_shared(); }
    void decrement_shared() { ctrl.decrement_shared(); }
    void increment_weak() { ctrl.increment_weak(); }
    void decrement_weak() { ctrl.decrement_weak(); }
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
private:
    using __shared_ptr_T = my::__shared_ptr<T, Ctrl>;
public:
    mock_shared_ptr() : __shared_ptr_T() { }
    explicit mock_shared_ptr(T* ptr) : __shared_ptr_T(ptr) { }
    // for mock unit testing
    const Ctrl* get_control_block() const noexcept
    {
        return __shared_ptr_T::get_control_block();
    }
};

template <typename T, typename Ctrl = mock_control_block>
class mock_weak_ptr : public my::__weak_ptr<T, Ctrl>
{
private:
    using __weak_ptr_T = my::__weak_ptr<T, Ctrl>;
    using __shared_ptr_T = mock_shared_ptr<T, Ctrl>;
public:
    mock_weak_ptr() : __weak_ptr_T() { }
    mock_weak_ptr(const __shared_ptr_T& other) noexcept : __weak_ptr_T(other) { }
    // for mock unit testing
    const Ctrl* get_control_block() const noexcept
    {
        return __weak_ptr_T::get_control_block();
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

TEST(WeakPtrTest, DestructorTest) 
{
    mock_weak_ptr<int> ptr;
    {
        mock_shared_ptr<int> s_ptr(new int(10));
        ptr = mock_weak_ptr<int>(s_ptr);
        mock_shared_ptr<int> s_ptr2(s_ptr);
        EXPECT_EQ(ptr.use_count(), 2);
    }
    // test control block destruction when weak_ptr is out of scope
    const mock_control_block* ctrl = ptr.get_control_block();
    EXPECT_NE(ctrl, nullptr);
    EXPECT_CALL(*ctrl, Die());
}