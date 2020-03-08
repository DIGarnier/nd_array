#include <iostream>
#include "nd_array.h"



int main(){
    constexpr int xMax = 3;
    constexpr int yMax = 2;
    constexpr int zMax = 2;

    auto test = nd_array<int, xMax, yMax, zMax>{    1,2,3,
                                                    4,5,6,

                                                    7,8,9,
                                                    10,11,12
                                               };
    
    for(auto e: test)
        std::cout << e << '\n';


    
    for(auto i = 0; i < test.size(); ++i){
        auto idx = i;
        int z = idx / (xMax * yMax);
        idx -= (z * xMax * yMax);
        int y = idx / xMax;
        int x = idx % xMax;
        std::cout << "x: " << x << " y: " << y << " z: " << z << " result: " << test.at(x, y, z) << '\n';
    }
    
}