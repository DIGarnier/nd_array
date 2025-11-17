#include <iostream>
#include <algorithm>
#include <numeric>
#include "nd_array.h"

int main()
{
    // Original test case
    constexpr int xMax = 3;
    constexpr int yMax = 2;
    constexpr int zMax = 2;
    constexpr auto ndarray = nd_array<int, xMax, yMax, zMax>{
        1,2,3,
        4,5,6,

        7,8,9,
        10,11,12
    };

    // Original static assertions
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

    // Test new features

    // 1. Test constexpr utility functions
    static_assert(ndarray.dimension_count() == 3);
    static_assert(ndarray.dimension_size(0) == 3);
    static_assert(ndarray.dimension_size(1) == 2);
    static_assert(ndarray.dimension_size(2) == 2);

    // 2. Test spaceship operator (C++20)
    constexpr auto ndarray2 = nd_array<int, 3, 2, 2>{
        1,2,3,
        4,5,6,
        7,8,9,
        10,11,12
    };
    static_assert(ndarray == ndarray2);
    static_assert(!(ndarray != ndarray2));

    // 3. Test with different types
    constexpr auto float_array = nd_array<double, 2, 2>{
        1.5, 2.5,
        3.5, 4.5
    };
    static_assert(float_array.at(0,0) == 1.5);
    static_assert(float_array.at(1,1) == 4.5);

    // 4. Test unsafe_at for performance-critical access
    static_assert(ndarray.unsafe_at(1, 1, 1) == 11);

    // 5. Runtime demonstrations
    std::cout << "nd_array modernization demo\n";
    std::cout << "============================\n\n";

    // Demonstrate iterator support (range-based for loop)
    std::cout << "All elements using range-based for: ";
    for (const auto& elem : ndarray) {
        std::cout << elem << " ";
    }
    std::cout << "\n\n";

    // Use STL algorithms thanks to iterator support
    std::cout << "Sum of all elements: "
              << std::accumulate(ndarray.begin(), ndarray.end(), 0) << "\n";

    std::cout << "Max element: "
              << *std::max_element(ndarray.begin(), ndarray.end()) << "\n";

    std::cout << "Min element: "
              << *std::min_element(ndarray.begin(), ndarray.end()) << "\n\n";

    // Demonstrate dimension info
    std::cout << "Dimension count: " << ndarray.dimension_count() << "\n";
    std::cout << "Dimension sizes: ";
    auto dims = ndarray.get_dimensions();
    for (std::size_t i = 0; i < dims.size(); ++i) {
        std::cout << dims[i];
        if (i < dims.size() - 1) std::cout << " x ";
    }
    std::cout << "\n\n";

    // Demonstrate mutable access
    auto mutable_array = nd_array<int, 2, 2>{
        1, 2,
        3, 4
    };

    std::cout << "Before modification:\n";
    std::cout << "mutable_array.at(0,0) = " << mutable_array.at(0,0) << "\n";

    mutable_array.at(0,0) = 99;

    std::cout << "After modification:\n";
    std::cout << "mutable_array.at(0,0) = " << mutable_array.at(0,0) << "\n\n";

    // Demonstrate comparison operators
    auto array_a = nd_array<int, 2, 2>{1, 2, 3, 4};
    auto array_b = nd_array<int, 2, 2>{1, 2, 3, 4};
    auto array_c = nd_array<int, 2, 2>{1, 2, 3, 5};

    std::cout << "Comparison tests:\n";
    std::cout << "array_a == array_b: " << (array_a == array_b ? "true" : "false") << "\n";
    std::cout << "array_a == array_c: " << (array_a == array_c ? "true" : "false") << "\n";
    std::cout << "array_a < array_c: " << (array_a < array_c ? "true" : "false") << "\n\n";

    // Demonstrate exception handling
    try {
        std::cout << "Attempting out-of-bounds access...\n";
        [[maybe_unused]] auto val = mutable_array.at(5, 5);
    } catch (const std::out_of_range& e) {
        std::cout << "Caught exception: " << e.what() << "\n\n";
    }

    std::cout << "All tests passed!\n";

    return 0;
}
