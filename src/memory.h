#pragma once
#include <cstdlib>
#include <utility>
#include <cstdint>

using std::size_t, std::align_val_t, std::uintptr_t;


namespace my 
{

// alignment al should be power of 2
void* new_aligned(size_t sz, align_val_t al)
{
    size_t align = (size_t)al;
    // min alignment should be at least one pointer (4/8 bytes for 32/64 bit arch)
    if (align < sizeof(void*))
        align = sizeof(void*);
    
    // extra align bytes to store the original value returned by malloc.
    void* const malloc_ptr = std::malloc(sz + align);
    if (!malloc_ptr)
        return nullptr;
    
    // align to the requested value, leaving room for the original malloc poionter.
    // align is at least one pointer, shifting by align will guarentee space for one pointer
    // we cast malloc_ptr to and integer type (uintptr_t) to do integer arthmetic instead of pointer arthmetic
    // ~(align - 1) is bitmask for alignment (align is power of 2)
    void* const aligned_ptr = (void*)( ((uintptr_t)malloc_ptr + align) & ~(align - 1) );

    // store the original malloc pointer where it can be found by operator delete.
    // we already shifted aligned_ptr to have space for one pointer behind it
    ( (void**)aligned_ptr )[-1] = malloc_ptr;

    return aligned_ptr;
}

void delete_aligned(void* ptr, std::align_val_t) noexcept
{
    if (ptr)
        std::free( ((void**)ptr)[-1] );   // See my::new_aligned
}


}   // namespace my



// TODO: test