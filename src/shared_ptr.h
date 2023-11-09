#pragma once
#include <cstdint>
#include <utility>

using std::size_t;

namespace my 
{

// forward declarations
    class __control_block;

template <typename T, typename Ctrl = __control_block>   // control block template arg needed for unit test mocks
    class __shared_ptr;
template <typename T, typename Ctrl = __control_block>   // control block template arg needed for unit test mocks
    class __weak_ptr;

template <typename T>
    class shared_ptr;
template <typename T>
    class weak_ptr;


// public interface
template <typename T>
class shared_ptr : public __shared_ptr<T>
{
public:
    shared_ptr() : __shared_ptr<T>() { }
    explicit shared_ptr(T* ptr) : __shared_ptr<T>(ptr) { }
    shared_ptr(__shared_ptr<T>&& other) : __shared_ptr<T>(other) { }   // for __weak_ptr.lock()
};


template <typename T>
class weak_ptr : public __weak_ptr<T>
{
public:
    weak_ptr() : __weak_ptr<T>() { }
    weak_ptr(const shared_ptr<T>& other) noexcept : __weak_ptr<T>(other) { }
};


// implementation details 
template <typename T, typename Ctrl>
class __shared_ptr
{
private:
    using __weak_ptr_T = __weak_ptr<T, Ctrl>;
    friend __weak_ptr_T;

public:
    constexpr __shared_ptr() noexcept
    {
    }

    explicit __shared_ptr(T* ptr)
        : m_ptr(ptr)
        , m_ctrl(new Ctrl)
    {
        m_ctrl->increment_shared();
    }
 
    __shared_ptr(const __shared_ptr& other) noexcept
        : m_ptr(other.m_ptr)
        , m_ctrl(other.m_ctrl)
    {
        if (m_ctrl)
            m_ctrl->increment_shared();
    }

    __shared_ptr(__shared_ptr&& other) noexcept
        : m_ptr(other.m_ptr)
        , m_ctrl(other.m_ctrl)
    {
        other.m_ptr = nullptr;
        other.m_ctrl = nullptr;
    }

    explicit __shared_ptr(const __weak_ptr_T& other)
        : m_ptr(other.m_ptr)
        , m_ctrl(other.m_ctrl)
    {
        if (m_ctrl)
            m_ctrl->increment_shared();
    }

    __shared_ptr& operator=(const __shared_ptr& other) noexcept
    {
        if (this != &other)
        {
            this->~__shared_ptr();
            m_ptr = other.m_ptr;
            m_ctrl = other.m_ctrl;
            if (m_ctrl)
                m_ctrl->increment_shared();
        }
        return *this;
    }

    __shared_ptr& operator=(__shared_ptr&& other) noexcept
    {
        if (this != &other)
        {
            this->~__shared_ptr();
            m_ptr = other.m_ptr;
            m_ctrl = other.m_ctrl;
            other.m_ptr = nullptr;
            other.m_ctrl = nullptr;
        }
        return *this;
    }

    ~__shared_ptr()
    {
        if (m_ctrl)
        {    
            m_ctrl->decrement_shared();
            if (m_ctrl->shareds() == 0)
                delete m_ptr;
            if (m_ctrl->refs() == 0)
                delete m_ctrl;
        }
    }
 
    void reset() noexcept
    {
        this->~__shared_ptr();
        m_ptr = nullptr;
        m_ctrl = nullptr;
    }

    void reset(T* ptr)
    {
        this->~__shared_ptr();
        m_ptr = ptr;
        m_ctrl = new Ctrl;
        m_ctrl->increment_shared();
    }

    // void swap(shared_ptr<T>& r) noexcept
    // {
    //     // this function requires a pointer to the control block
    // }

    size_t use_count() const
    {
        if(m_ctrl)
            return m_ctrl->shareds();
        return 0;
    }
 
    T* get() const noexcept
    {
        if (m_ptr)
            return m_ptr;
        return 0;
    }
 
    T& operator*() const noexcept
    {
        return *get();
    }
 
    T* operator->() const noexcept
    {
        return get();
    }

private:
    T* m_ptr = nullptr;
    Ctrl* m_ctrl = nullptr;

// for mock unit testing
protected:
    const Ctrl* get_control_block() const noexcept
    {
        return m_ctrl;
    }
};


template <typename T, typename Ctrl>
class __weak_ptr
{
private:
    using __shared_ptr_T = __shared_ptr<T, Ctrl>;
    friend __shared_ptr_T;

public:
    constexpr __weak_ptr() noexcept
    {
    }

    __weak_ptr(const __weak_ptr& other) noexcept
        : m_ptr(other.m_ptr)
        , m_ctrl(other.m_ctrl)
    {
        if (m_ctrl)
            m_ctrl->increment_weak();
    }

    __weak_ptr(__weak_ptr&& other) noexcept
        : m_ptr(other.m_ptr)
        , m_ctrl(other.m_ctrl)
    {
        other.m_ptr = nullptr;
        other.m_ctrl = nullptr;
    }
    
    __weak_ptr(const __shared_ptr_T& other) noexcept
        : m_ptr(other.m_ptr)
        , m_ctrl(other.m_ctrl)
    {
        if (m_ctrl)
            m_ctrl->increment_weak();
    }

    __weak_ptr& operator=(const __weak_ptr& other) noexcept
    {
        if (this != &other)
        {
            this->~__weak_ptr();
            m_ptr = other.m_ptr;
            m_ctrl = other.m_ctrl;
            if (m_ctrl)
                m_ctrl->increment_weak();
        }
        return *this;
    }

    __weak_ptr& operator=(__weak_ptr&& other) noexcept
    {
        if (this != &other)
        {
            this->~__weak_ptr();
            m_ptr = other.m_ptr;
            m_ctrl = other.m_ctrl;
            other.m_ptr = nullptr;
            other.m_ctrl = nullptr;
        }
        return *this;
    }

    ~__weak_ptr()
    {
        if (m_ctrl)
        {    
            m_ctrl->decrement_weak();
            if (m_ctrl->refs() == 0)
                delete m_ctrl;
        }
    }

    size_t use_count() const noexcept
    {
        if(m_ctrl)
            return m_ctrl->shareds();
        return 0;
    }

    bool expired() const noexcept
    {
        return use_count() == 0;
    }

    __shared_ptr_T lock() const noexcept
    {
        return expired() ? __shared_ptr_T() : __shared_ptr_T(*this);
    }

private:
    T* m_ptr = nullptr;
    Ctrl* m_ctrl = nullptr;

// for mock unit testing
protected:
    const Ctrl* get_control_block() const noexcept
    {
        return m_ctrl;
    }
};


class __control_block
{
public:
    void increment_shared()
    {
        ++m_shareds;
        ++m_refs;
    }

    void increment_weak()
    {
        ++m_refs;
    }

    void decrement_shared()
    {
        --m_shareds;
        --m_refs;
    }

    void decrement_weak()
    {
        --m_refs;
    }

    size_t shareds() const noexcept
    {
        return m_shareds;
    }
    
    size_t refs() const noexcept
    {
        return m_refs;
    }

private:
    size_t m_shareds = 0;
    size_t m_refs = 0;
};

// template <typename T>
// shared_ptr<T> make_shared()
// {

// }


}    // namespace my





// TODO:
// - make shared fuction
// - swap/reset function