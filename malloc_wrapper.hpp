#ifndef ___MALLOC_WRAPPER__HPP___
#define ___MALLOC_WRAPPER__HPP___

#include <cstdlib>
#include <limits>

template <typename T> class malloc_wrapper;

template <> class malloc_wrapper<void>
{
public:
    typedef void* pointer;
    typedef const void* const_pointer;
    // reference to void members are impossible.
    typedef void value_type;
    template <class U> 
        struct rebind { typedef malloc_wrapper<U> other; };
};    

namespace malloc_wrap{
    inline void destruct(char *){}
    inline void destruct(wchar_t*){}
    template <typename T> 
        inline void destruct(T *t){t->~T();}
} // namespace
    
template <typename T>
class malloc_wrapper
{
public:
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T value_type;

    template <class U> 
        struct rebind { typedef malloc_wrapper<U> other; };
    malloc_wrapper(){}
    pointer address(reference x) const { return &x; }
    const_pointer address(const_reference x) const { return &x; }
    pointer allocate(size_type size, malloc_wrapper<void>::const_pointer hint = 0) {
        int bytes = size * sizeof(T);
        std::cout << "Allocating " << bytes << " bytes." << std::endl;
        return static_cast<pointer>(malloc(bytes));
    }

    template <class U> malloc_wrapper(const malloc_wrapper<U>&){}
    void deallocate(pointer p, size_type n) {
        free(p);
    }
    void deallocate(void *p, size_type n) {
        free(p);
    }
    size_type max_size() const throw() {
        return std::numeric_limits<size_t>::max()/sizeof(T);
    }
    void construct(pointer p, const T& val)
    {
        new(static_cast<void*>(p)) T(val);
    }
    void construct(pointer p)
    {
        new(static_cast<void*>(p)) T();
    }
    void destroy(pointer p){malloc_wrap::destruct(p);}
#if 0
private:
    static pool mem_;
#endif
};


template <typename T, typename U>
inline bool operator==(const malloc_wrapper<T>&, const malloc_wrapper<U>){return true;}

template <typename T, typename U>
inline bool operator!=(const malloc_wrapper<T>&, const malloc_wrapper<U>){return false;}


namespace std{
template <class _Tp1, class _Tp2>
inline malloc_wrapper<_Tp2>&
__stl_alloc_rebind(malloc_wrapper<_Tp1>& __a, const _Tp2*) 
{  
    return (malloc_wrapper<_Tp2>&)(__a); 
}


template <class _Tp1, class _Tp2>
inline malloc_wrapper<_Tp2>
__stl_alloc_create(const malloc_wrapper<_Tp1>&, const _Tp2*) 
{ 
    return malloc_wrapper<_Tp2>(); 
}

} // namespace std
// end STLPort

#endif

