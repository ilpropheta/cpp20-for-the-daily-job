// C++20 for the daily job (Marco Arena)
//
// <bit>

#include <iostream>
#include <algorithm>
#include <bit>

using namespace std;

struct two_ints
{
    int i1, i2;
};

int main()
{
    cout << "Number of ones in 12: " << popcount(12u) << "\n";
    cout << "Number of bits needed to represent 12: " << bit_width(12u) << "\n";
    
    auto maxValue = numeric_limits<unsigned long long>::max();
    // suppose we use two 32-bit integers to store a 64-bit unsigned integer
    auto storage = bit_cast<two_ints>(maxValue);
    auto maxValueBack = bit_cast<unsigned long long>(storage);
    
    cout << "Value back is the same: " << boolalpha << (maxValueBack == maxValue);
}
