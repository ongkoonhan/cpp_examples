#pragma once
#include <cstdint>
#include <utility>

using std::size_t;


namespace my 
{

template <typename T>
class vector
{
public:
    vector()
    {
        __realloc(2);   // start with 2
    }

    // vector(const vector& other)
    //     : m_size(other.m_size)
    //     , m_capacity(other.m_capacity)
    //     , m_arr(new T[other.m_capacity])
    // {
    //     for (size_t i = 0; i < m_capacity; ++i)
    //         m_arr[i] = other.m_arr[i];
    // }

    ~vector()
    {
        clear();   // destroy all objects
        ::operator delete(m_arr, m_capacity * sizeof(T));
    }

    void push_back(const T& val)
    {
        __capacityCheck();        
        m_arr[m_size] = val;
        m_size++;
    }

    void push_back(T&& val)
    {
        __capacityCheck();
        m_arr[m_size] = std::move(val);
        m_size++;
    }

    template <typename... Args>
    T& emplace_back(Args&&... args)
    {
        __capacityCheck();
        new(&m_arr[m_size]) T(std::forward<Args>(args)...);   // construct at array address with placement new
        m_size++;
        return m_arr[m_size-1];
    }

    void pop_back()
    {
        if (m_size > 0)
        {
            m_size--;
            m_arr[m_size].~T();
        }
    }

    void clear()
    {
        for (size_t i = 0; i < m_size; i++)
            m_arr[i].~T();
        m_size = 0;
    }

    T& operator[](size_t i)
    {
        return m_arr[i];
    }

    const T& operator[](size_t i) const
    {
        return m_arr[i];
    }

    size_t size() const
    {
        return m_size;
    }

private:
    void __realloc(size_t newCapacity)
    {
        size_t newSize = newCapacity < m_size ? newCapacity : m_size;
        // ::operator new to allocate memory without calling constructor
        // cast void* to T*
        T* newArr = (T*) ::operator new(newCapacity * sizeof(T));   
        // placement new at array address with uinitialized memory
        // use move constructor if possible
        for (size_t i = 0; i < newSize; i++)
            new(&newArr[i]) T(std::move(m_arr[i]));
        // destroy all existing objects in m_arr before deleting the memory
        // deallocate with ::operator delete
        for (size_t i = 0; i < m_size; i++)
            m_arr[i].~T();
        ::operator delete(m_arr, m_capacity * sizeof(T));
        
        m_size = newSize;
        m_arr = newArr;
        m_capacity = newCapacity;
    }

    void __capacityCheck()
    {
        if (m_size >= m_capacity)
            __realloc(m_capacity + m_capacity / 2);   // grow by 50%
    }
    

private:
    size_t m_size = 0;
    size_t m_capacity = 0;
    T* m_arr = nullptr;
};

}   // namespace my


// #include <vector>
// std::vector<int> a;
// a.reserve(10);