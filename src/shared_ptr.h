#pragma once
#include <cstdint>
#include <utility>

using std::size_t;

namespace my 
{

template <typename T>
class shared_ptr
{
public:
    shared_ptr()
    {
    }

    explicit shared_ptr(T* ptr)
        : m_ptr(ptr)
        , m_counter(new size_t(1))
    {
    }
 
    shared_ptr(const shared_ptr<T>& other) noexcept
        : m_ptr(other.m_ptr)
        , m_counter(other.m_counter)
    {
        if (m_counter)
            ++(*m_counter);
    }

    shared_ptr(shared_ptr<T>&& other) noexcept
        : m_ptr(other.m_ptr)
        , m_counter(other.m_counter)
    {
        other.m_ptr = nullptr;
        other.m_counter = nullptr;
    }

    // TODO: copy and move assignment

    ~shared_ptr()
    {
        if (m_counter)
        {    
            --(*m_counter);
            if (*m_counter == 0)
            {
                delete m_ptr;
                delete m_counter;
            }
        }
    }
 
    void reset() noexcept
    {
        this->~shared_ptr();
        m_ptr = nullptr;
        m_counter = nullptr;
    }

    void reset(T* ptr)
    {
        this->~shared_ptr();
        m_ptr = ptr;
        m_counter = new size_t(1);
    }

    // void swap(shared_ptr<T>& r) noexcept
    // {
    //     // this function requires a pointer to the control block
    // }

    size_t use_count() const
    {
        if(m_counter)
            return *m_counter;
        return 0;
    }
 
    T* get()
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
    size_t* m_counter = nullptr;
};

// template <typename T>
// shared_ptr<T> make_shared()
// {

// }


}    // namespace my





// TODO:
// - make shared fuction
// - swap/reset function