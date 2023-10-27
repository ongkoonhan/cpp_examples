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
template <typename T, typename Ctrl>
    class __weak_ptr;

template <typename T>
    class weak_ptr;
template <typename T>
    class shared_ptr;


// public interface
template <typename T>
class shared_ptr : public __shared_ptr<T>
{
public:
    shared_ptr() : __shared_ptr<T>() { }
    explicit shared_ptr(T* ptr) : __shared_ptr<T>(ptr) { }
};


// implementation details 
template <typename T>
class weak_ptr
{

};


template <typename T, typename Ctrl>
class __shared_ptr
{
public:
    __shared_ptr()
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

class __control_block
{
public:
    void increment_shared()
    {
        ++m_shareds;
        ++m_refs;
    }

    void decrement_shared()
    {
        --m_shareds;
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