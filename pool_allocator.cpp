#include "pool_allocator.h"
#include "ss_allocator.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include "timer.h"
#include <string>
#include <ctime>
using namespace std;

const int size = 10000;

struct foo
{
    foo(const string &s = "Hello world", int i = 0):s_(s), i_(i){}
    string s_;
    int i_;
    bool operator <(const foo& other)const{return i_<other.i_;}
};

foo generator()
{
    return foo("Hello world", rand());
}



int main() 
{
    timer t;
    srand(time(0));
    cout.precision(10); 
    cout<<"***Tests with <vector>***"<<endl;
    t.elapsed();
    vector<foo, pool_allocator<foo> > pool_vector(size);
    cout<<"Construction (pool_allocator):"<<t.elapsed()<<endl;
    vector<foo, ss_allocator<foo> > ss_vector(size);
    cout<<"Construction (ss_allocator):"<<t.elapsed()<<endl;
    vector<foo> default_vector(size);
    cout<<"Construction (default allocator):"<<t.elapsed()<<endl;
    generate(pool_vector.begin(), pool_vector.end(), generator);
    copy(pool_vector.begin(), pool_vector.end(), default_vector.begin());
    copy(pool_vector.begin(), pool_vector.end(), ss_vector.begin());

    t.elapsed();
    sort(pool_vector.begin(), pool_vector.end());
    cout<<"Sort (pool_allocator):"<<t.elapsed()<<endl;
    sort(ss_vector.begin(), ss_vector.end());
    cout<<"Sort (ss_allocator):"<<t.elapsed()<<endl;
    sort(default_vector.begin(), default_vector.end());
    cout<<"Sort (default allocator):"<<t.elapsed()<<endl;


    cout<<"***Tests with <list>***"<<endl;
    t.elapsed();
    list<foo, pool_allocator<foo> > pool_list(size);
    cout<<"Construction (pool_allocator):"<<t.elapsed()<<endl;
    list<foo, ss_allocator<foo> > ss_list(size);
    cout<<"Construction (ss_allocator):"<<t.elapsed()<<endl;
    list<foo> default_list(size);
    cout<<"Construction (default allocator):"<<t.elapsed()<<endl;
    generate(pool_list.begin(), pool_list.end(), generator);
    copy(pool_list.begin(), pool_list.end(), default_list.begin());
    copy(pool_list.begin(), pool_list.end(), ss_list.begin());
    t.elapsed();
    pool_list.sort();
    cout<<"Sort (pool_allocator):"<<t.elapsed()<<endl;
    ss_list.sort();
    cout<<"Sort (ss_allocator):"<<t.elapsed()<<endl;
    default_list.sort();
    cout<<"Sort (default allocator):"<<t.elapsed()<<endl;
    cout<<"Press <Enter>..."<<endl;
    char c;
    cin.getline(&c, 1);
    return 0;
}


