# nd_array

Really simple n-dimensional array.

```c++
constexpr int xMax = 2;
constexpr int yMax = 2;
constexpr int zMax = 2;
constexpr auto ndarray = nd_array<int, xMax, yMax, zMax>{
      1,2,
      3,4,

      5,6,
      7,8,
};

static_assert(ndarray.at(0,0,0) == 1);
static_assert(ndarray.at(1,0,0) == 2);
static_assert(ndarray.at(0,1,0) == 3);
static_assert(ndarray.at(1,1,0) == 4);

static_assert(ndarray.at(0,0,1) == 5);
static_assert(ndarray.at(1,0,1) == 6);
static_assert(ndarray.at(0,1,1) == 7);
static_assert(ndarray.at(1,1,1) == 8);

```
