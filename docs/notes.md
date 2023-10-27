

## Cache types and latency
* Instruction cache
* Data cache
* Translation lookaside buffer
* Policies
    * Replacement policy (make room for new cache lines)
        * e.g. least-recently used (LRU)
    * Write policy (writing cache changes back to main memory)
        * write-through (write to memory on every cache write)
        * write-back (track cache changes (dirty lines), write to memory only when evicted)
* Cache coherence
    * maintaining a coherent view of local caches when reads/writes are performed
    * e.g. multiprocessor CPU system with multiple local caches
* Cache associativity
    * how memory locations are mapped to cache locations
    * Placement policy (examples)
        * Direct mapped (each memory location can only go to one cache location)
        * Two-way associative (each memory location can only go to one of two cache locations)
* https://en.wikipedia.org/wiki/CPU_cache


## Networking and exchange protocol basics
* TCP vs UDP
    * TCP uses a connection, UDP has no connection
        * TCP uses 3 way handshake (SYN, SYN-ACK, ACK) to establish connection
    * TCP tracks the data sequence, UDP doesn't
    * TCP has error checking, UDP doesn't
    * UDP can broadcast/multicast
    * header size: TCP -> 20-60 bytes, UDP -> 8 bytes
    * https://www.avast.com/c-tcp-vs-udp-difference
        * https://www.geeksforgeeks.org/differences-between-tcp-and-udp/
* broadcast vs multicast
    * broadcast is to all hosts
    * multicast is to selected hosts
* FIX protocol
    * 
    * https://www.fixtrading.org/online-specification/
* More details on TCP (to add to notes)
    * https://packetlife.net/blog/2010/jun/7/understanding-tcp-sequence-acknowledgment-numbers/


## C++ topics
* virtual
    * what is a virtual function?
        * allows for a funtion to have runtime polymorphism
        * derived class funtions marked as virtual called at runtime even though pointer/ref is of base type
        * normal funtion calls are bound at compile time, while virtual functions are bound at runtime
        * implicitly uses a pointer to a vtable of virtual functions
    * how do virtual destructors work?
        * destructor for derived class called even though pointer/ref is of base type
        * virtual destructor should always be used when one or more functions are marked as virtual
* memory allocation/deallocation
    * expression new
        * create (array of) objects with dynamic storage duration
    * operator new
        * attempts to allocate requested number of bytes (can throw if allocation fails)
            * might use malloc inside the implementation (no guarentees)
        * `void* operator new  ( std::size_t count );`
            * `::operator new()` and `::operator new[]()` are the same in STL
        * Can be replaced by user
        * https://en.cppreference.com/w/cpp/memory/new/operator_new
    * https://learn.microsoft.com/en-us/cpp/standard-library/new-operators?view=msvc-170



* miscellaneous
    * function pointer
        * declaration: 
            ```
            return_type (*FuncPtr) (parameter type, ....);
            ```
        * examples and variations
            ```
            // basic version
            int (*func)(int, int) --> int func(int, int)
            
            // nicer syntax
            using func = int(*)(int, int) --> int func(int, int)
            
            // const member function pointer
            int (SomeClass::*func)(int, int) const --> int SomeClass::func(int, int) const
            ```
        * example
            ```
            int multiply(int a, int b) { return a * b; }
            ...
            int main()
            {
                int (*func)(int, int);    
                func = multiply;
                ...
            }
            ```

* const vs constexpr
* lvalue vs rvalue
* threading, mutex
* exceptions
* templates
* atomics
* sockets


## Generic questions
* Compression algo
* Computer architecture
* what is a singleton?
* multithreading vs multiprocessing
* what is a segfault?



# TODO
* operator new/delete
* placement new
* malloc/free vs new/delete