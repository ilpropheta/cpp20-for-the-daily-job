// C++20 for the daily job (Marco Arena)
//
// std::span

#include <iostream>
#include <span>
#include <string>
#include <vector>
#include <list>
#include <ranges>
#include <algorithm>
#include <array>
#include <bitset>

using namespace std;

template<typename T, size_t Extent>
void passMeAnySpan(span<T, Extent> s)
{
    for (auto i : s)
        cout << i << " ";
    cout << "\n";
}

void c_style_pass(float* coeffs, int N)
{
    for(auto i=0; i<N; ++i)
        cout << coeffs[i] << " ";
    cout << "\n";
}

void modern_style_pass(span<const float> coeffs)
{
    for (auto i : coeffs)
        cout << i << " ";
    cout << "\n";
}

int main()
{
    vector<string> pirateCoves = {"tortuga", "kingston", "nassau", "havana", "st. kitts"};
    // try to use list instead...
           
    // fixed span
    span<const string, 3> threeCoves{ pirateCoves.begin(), 3 };
    static_assert(threeCoves.size()==3);
    cout << "sizeof fixed span: " << sizeof(threeCoves) << "\n"; // size of fixed spans is part of the type
        
    // dynamic span
    span allCoves = pirateCoves; // CTAD automatically deduces extent=dynamic_extent
    cout << "sizeof dynamic span: " << sizeof(allCoves) << "\n"; // dynamic spans are, basically, [pointer, size]
    
    // automatic deduction of fixed span
    int cannons[5] = {5, 8, 2, 15, 6};
    span cannonSpan = cannons; // CTAD automatically deduces extent=5 (same story for std::array)
    static_assert(cannonSpan.size()==5);
    
    // subspan
    auto twoCannons1 = cannonSpan.subspan(0, 2);
    cout << "extent of twoCannons1: " << decltype(twoCannons1)::extent << "\n";
    
    auto twoCannons2 = cannonSpan.subspan<0, 3>(); // try <0, 6>
    cout << "extent of twoCannons2: " << decltype(twoCannons2)::extent << "\n";
    
    // constexpr subspan(off, count) does compile-time check but the returning extent is still dynamic...
    static constexpr array crew = { 25, 36, 18, 65 };
	  constexpr std::span crewSpan = crew;
    constexpr auto crewSubSpan = crewSpan.subspan(4); // try with 5 or more
	  cout << "extent of crewSubSpan: " << decltype(crewSubSpan)::extent << "\n";
    
    // span is a borrowed_range
    cout << "is nassau in top 3 coves? " << ranges::count(span{begin(pirateCoves), 3}, "nassau") << "\n";
}
 
