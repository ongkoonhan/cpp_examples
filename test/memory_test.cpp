#include "memory.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <sanitizer/lsan_interface.h>

#include <thread>
#include <fcntl.h>


namespace
{

bool check_memory_leak()
{   
    // we use the leak sanitizer leak check, but redirect output to /dev/null
    // AddressSanitizer prints using C, not C++ (hence the C-style of redirecting)

    int stderr_copy = dup(STDERR_FILENO);   // keep copy of stderr fd
    int fd = open( "/dev/null", O_WRONLY );   // /dev/null fd
    
    dup2( fd, STDERR_FILENO );   // redirect
    int is_leak = __lsan_do_recoverable_leak_check();   // leak check prints to stderr on every call
    dup2( stderr_copy, STDERR_FILENO );   // restore stderr fd (undo redirect)
    // clean up
    close(fd);
    close(stderr_copy);
    
    return is_leak;
}

void test_wrapper(std::function<void()>&& f)
{
    static bool skip_ = false;

    // skip future tests if one fails
    // AddressSanitizer leak sanitizer detects leaks on the whole program (static/global check)
    // if a leak is detected in one test, there is no point continuing the other tests as we 
    //   can't differentiate between two different leaks from the lsan_interface alone
    if (skip_)
        GTEST_SKIP();
    
    // start in thread
    std::thread t(f);
    if (t.joinable())
        t.join();

    // check for memory leak from thread
    if (check_memory_leak())
    {
        skip_ = true;
        FAIL();
    }
}

}   // anonymous namespace


using std::align_val_t, std::size_t;

TEST(MemoryTest, AlignTest64Bit)
{
    test_wrapper([]()
    {
        size_t align = 8;   // bytes
        void* aligned_ptr = my::new_aligned(align * 2, align_val_t(align));
        my::delete_aligned(aligned_ptr, align_val_t(align));
    });
}

TEST(MemoryTest, AlignTest32Byte)
{
    test_wrapper([]()
    {
        size_t align = 32;   // bytes
        void* aligned_ptr = my::new_aligned(align * 10, align_val_t(align));
        my::delete_aligned(aligned_ptr, align_val_t(align));
    });
}
