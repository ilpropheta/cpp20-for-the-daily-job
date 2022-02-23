// C++20 for the daily job (Marco Arena)
//
// comparisons and operator<=>

#include <iostream>
#include <compare>
#include <tuple>
#include <strings.h>

using namespace std;

struct Point3D 
{
	auto operator<=> (const Point3D& other) const = default;
    int x, y, z;    
};

struct CIString
{
    std::string str;
    
    std::weak_ordering operator<=>(const CIString& right) const
    { 
       const auto cmp = strncasecmp(str.data(), right.str.data(), std::min(str.size(), right.str.size()));
	   return cmp == 0 ? (str.size() <=> right.str.size()) : cmp <=> 0;
    };
    
    bool operator==(const CIString& right) const    
    {
        return *this <=> right == 0;
    }
    
    // !!! WARNING: 
    /*
        bool operator==(const CIString& right) const = default;
    
    IS NOT THE SAME AS
    
        bool operator==(const CIString& right) const    
        {
            return *this <=> right == 0;
        }
    
    instead, 
        bool operator==(const CIString& right) const = default;
        
        generates an operator performing a memberwise comparison!
    */
};

struct TrackedValue
{
    TrackedValue(std::string name, long i) noexcept
		: m_name{std::move(name)}, m_id{i}
	{
		static int _tracker = 0;
		m_trackingNumber = _tracker++;
	}

    auto operator<=> (const TrackedValue& other) const
	{
        // simple way to exclude (and reorder) members from the comparison
		return std::tie(m_id, m_name) <=> std::tie(other.m_id, other.m_name);
	}

	bool operator==(const TrackedValue& other) const
	{
		return *this <=> other == 0;
	}
    
    auto getTrackingNumber() const
    {
        return m_trackingNumber;
    }
private:
	std::string m_name;
    long m_id;
	int m_trackingNumber = 0;
};

int main()
{
    constexpr Point3D p1{10, 10, 1};
    constexpr Point3D p2{10, 10, 2};
    static_assert(p1 < p2);
    
    TrackedValue tv1{"marco", 5};
    TrackedValue tv2{"marco", 5};
    cout << boolalpha << "tracked values are equal:   " << (tv1==tv2) << "\n";
    cout << "tracking numbers are different:   " << (tv1.getTrackingNumber() != tv2.getTrackingNumber()) << "\n";
    
    CIString ci1{"Abc"};
    CIString ci2{"ABC"};
    cout << "strings are equal, ignoring case:   " << (ci1 == ci2) << "\n";
    
    CIString ci3{"Abc"};
    CIString ci4{"ABCD"};
    cout << "Abc is less than ABCD, ignoring case:   " << (ci3 < ci4);
}
