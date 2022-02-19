// C++20 for the daily job (Marco Arena)
//
// NTTP

#include <iostream>
#include <string_view>

using namespace std;

struct Discount
{
    double value;
};

template<Discount D>
auto CalculateTotalPrice(int price)
{
    return price * D.value;
}

template<double D>
auto CalculateTotalPrice(int price)
{
    return price * D;
}

template<auto D>
auto CalculateTotalPriceWithLambdaDiscount(int price)
{
    return price * D();
}

auto lambdaDiscount = []{
    return 0.5;
};

template<decltype(lambdaDiscount) D>
auto CalculateTotalPriceWithLambdaDiscount_Alternative(int price)
{
    return price * D();
}

// courtesy of "C++20 - The Compete Guide" by Nicolai Josuttis
// A simple way to pass string literals as template arguments

template<auto Prefix>
class Logger 
{
public:
    void log(string_view msg) const 
    {
        cout << Prefix << msg << "\n";
    }
};

template<std::size_t N>
struct Str 
{
    char chars[N];
    
    constexpr auto value() const
    {
        return chars;
    }
    
    friend std::ostream& operator<<(std::ostream& strm, const Str& s) 
    {
        return strm << s.chars;
    }
};

template<size_t N> Str(const char(&)[N]) -> Str<N>;

int main()
{
    cout << CalculateTotalPrice<Discount{0.9}>(35) << "\n";
    cout << CalculateTotalPrice<0.7>(35) << "\n";
    cout << CalculateTotalPriceWithLambdaDiscount<lambdaDiscount>(35) << "\n";
    cout << CalculateTotalPriceWithLambdaDiscount_Alternative<lambdaDiscount>(35) << "\n";
    
    Logger<Str{"cli> "}> ll;
    
    ll.log("this will be logged");
}
