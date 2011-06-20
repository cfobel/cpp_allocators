#include <vector>
#include <iostream>
#include <algorithm>
#include "timer.h"
#include "malloc_wrapper.hpp"
#include <string>
#include <ctime>
using namespace std;

int main(int argc, char **argv) {
    cout.precision(10); 

    const int size = atoi(argv[1]);

    vector<int, malloc_wrapper<int> > test(size);

    for(int i = 0; i < size; i++) {
        test[i] = i;
    }

    return 0;
}
