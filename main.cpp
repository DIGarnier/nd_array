#include <iostream>
#include "nd_array.h"


int main()
{   
    constexpr int xMax = 3;
    constexpr int yMax = 2;
    constexpr int zMax = 2;
    constexpr auto ndarray = nd_array<int, xMax, yMax, zMax>{
        1,2,3,
        4,5,6,

        7,8,9,
        10,11,12
    };

    static_assert(ndarray.at(0,0,0) == 1);
    static_assert(ndarray.at(1,0,0) == 2);
    static_assert(ndarray.at(2,0,0) == 3);

    static_assert(ndarray.at(0,1,0) == 4);
    static_assert(ndarray.at(1,1,0) == 5);
    static_assert(ndarray.at(2,1,0) == 6);

    static_assert(ndarray.at(0,0,1) == 7);
    static_assert(ndarray.at(1,0,1) == 8);
    static_assert(ndarray.at(2,0,1) == 9);

    static_assert(ndarray.at(0,1,1) == 10);
    static_assert(ndarray.at(1,1,1) == 11);
    static_assert(ndarray.at(2,1,1) == 12);

    return 0;
}     
