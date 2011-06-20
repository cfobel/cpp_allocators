#ifndef SS_ALLOCATOR_H_INCLUDED_GF
#define SS_ALLOCATOR_H_INCLUDED_GF

#include "ss_storage.h"


template <typename T> class ss_allocator;
template <> class ss_allocator<void>
{
public:
    typedef void* pointer;
    typedef const void* const_pointer;
    // reference to void members are impossible.
    typedef void value_type;
    template <class U> 
        struct rebind { typedef ss_allocator<U> other; };
};    

namespace ss_alloc{
    inline void destruct(char *){}
    inline void destruct(wchar_t*){}
    template <typename T> 
        inline void destruct(T *t){t->~T();}
} // namespace ss_alloc
    
template <typename T>
class ss_allocator
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
        struct rebind { typedef ss_allocator<U> other; };

    ss_allocator(){}
    pointer address(reference x) const {return &x;}
    const_pointer address(const_reference x) const {return &x;}
    pointer allocate(size_type size, ss_allocator<void>::const_pointer hint = 0) const
    {
        if(size == 1) return mem_.allocate();
        return static_cast<pointer>(mem_.allocate(size*sizeof(T)));
    }

    // For Dinkumware (VC6SP5):
    char *_Charalloc(size_type n){return static_cast<char*>(mem_.allocate(n));}
    // end Dinkumware

    template <class U> ss_allocator(const ss_allocator<U>&){}
    ss_allocator(const ss_allocator<T>&){}
    void deallocate(pointer p, size_type n) const
    {
        mem_.deallocate(p, n);
    }
    void deallocate(void *p, size_type n) const
    {
        mem_.deallocate(p, n);
    }
    size_type max_size() const throw() {return size_t(-1) / sizeof(value_type);}
    void construct(pointer p, const T& val)
    {
        new(static_cast<void*>(p)) T(val);
    }
    void construct(pointer p)
    {
        new(static_cast<void*>(p)) T();
    }
    void destroy(pointer p){ss_alloc::destruct(p);}
    static void dump() {mem_.dump();}
private:
    
    static ss_storage<T> mem_;
};

template <typename T> ss_storage<T> ss_allocator<T>::mem_;

template <typename T, typename U>
inline bool operator==(const ss_allocator<T>&, const ss_allocator<U>){return true;}

template <typename T, typename U>
inline bool operator!=(const ss_allocator<T>&, const ss_allocator<U>){return false;}




// For VC6/STLPort 4-5-3 see /stl/_alloc.h, line 464
// "If custom allocators are being used without member template classes support :
// user (on purpose) is forced to define rebind/get operations !!!"
#ifdef _WIN32
#define SS_ALLOC_CDECL __cdecl
#else
#define SS_ALLOC_CDECL
#endif

namespace std{
template <class _Tp1, class _Tp2>
inline ss_allocator<_Tp2>& SS_ALLOC_CDECL
__stl_alloc_rebind(ss_allocator<_Tp1>& __a, const _Tp2*) 
{  
    return (ss_allocator<_Tp2>&)(__a); 
}


template <class _Tp1, class _Tp2>
inline ss_allocator<_Tp2> SS_ALLOC_CDECL
__stl_alloc_create(const ss_allocator<_Tp1>&, const _Tp2*) 
{ 
    return ss_allocator<_Tp2>(); 
}

} // namespace std
// end STLPort


#endif


