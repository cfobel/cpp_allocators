#ifndef SS_STORAGE_H_INCLUDED_GF
#define SS_STORAGE_H_INCLUDED_GF
#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable:4018) //signed/unsigned mismatch
#pragma warning(disable:4290) // exception spec ignored
#endif

#include <new>
#include <list>
#include <algorithm>
#include <iostream> //for dump()

template <typename T>
class ss_storage
{
    enum ss_defaults{init_size = 0xfffff};
public:
    ss_storage():size_(sizeof(link)>sizeof(T)?init_size*sizeof(link):init_size*sizeof(T)),
        step_(sizeof(link)>sizeof(T)?sizeof(link):sizeof(T))
    {
        ss_mem_.push_back(new char[size_]);
        link *l;
        head_ = l = reinterpret_cast<link *>(*ss_mem_.begin());
        for(int i = 1; i<init_size; ++i){
            l->next_ = reinterpret_cast<link *>(*ss_mem_.begin() + i*step_);
            l = l->next_;
        }
        l->next_ = 0;
    }
    
    T* allocate()
    {
        link *l = head_;
        if(!l) grow();
        head_ = head_->next_;
        return reinterpret_cast<T *>(l);
    }
    void* allocate(size_t n)
    {
        link *l = head_, *conn, *ret;       
        if(n%step_) n = step_*(n/step_+1);
        while(1){
            if(verify_memory_contiguous(l->next_, n/step_)){
                conn = ret = l->next_;
                for(unsigned int i=0; i<n/step_; ++i) conn = conn->next_;
                l->next_ = conn;
                return ret;
            }
            if(l->next_) l = l->next_;
            else {
                grow();
                l = head_;
            }
        }
    }
    void deallocate(void *p, size_t n)
    {
        if(n<=step_){
            link *l = head_;
            head_ = reinterpret_cast<link*>(p);
            head_->next_=l;
        }
        else{
            link *l = head_, *conn;
            head_ = reinterpret_cast<link*>(p);
            conn = head_;
            for(unsigned int i=0; i<n/step_; ++i){
                conn->next_ = reinterpret_cast<link*>(p) + i;
                conn = conn->next_;
            }
            conn->next_ = l;
        }
    }
    void dump()
    {
        using namespace std;
        link *l = head_;
        int links = 0;
        size_t free_space = 0;
        while(l->next_){
            free_space += step_;
            ++links;
            l = l->next_;
        }
        cout<<"Free space: "<<links*step_<<", links: "<<links<<endl;
    }
    ~ss_storage()
    {
        std::for_each(ss_mem_.begin(), ss_mem_.end(), killer());
    }

private:
    struct link
    {
        link *next_;
    };
    void grow()
    {
        char *new_chunk = new char[size_];
        ss_mem_.push_back(new_chunk);
        link *old_head = head_;
        link *l = reinterpret_cast<link *>(new_chunk);
        head_ = l;
        for(int i = 1; i<init_size; ++i){
            l->next_ = reinterpret_cast<link *>(new_chunk + i*step_);
            l = l->next_;
        }
        l->next_ = old_head;
    }
    bool verify_memory_contiguous(link *l, int n)
    {
        if(!l) return false;
        for(int i=0; i<n; ++i){
            if(l->next_){
                if(reinterpret_cast<char*>(l->next_) - reinterpret_cast<char*>(l) == step_){
                    l = l->next_;
                }
                else{
                    return false;
                }
            }
            else{
                return false;
            }
        }
        return true;
    }
    struct killer
    {
        void operator()(char *p){delete [] p;}
    };
    size_t size_;
    size_t step_;
    std::list<char *> ss_mem_;
    link *head_;

};
#ifdef _WIN32
#pragma warning(pop)
#endif

#endif

