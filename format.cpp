// C++20 for the daily job (Marco Arena)
//
// std::format

#include <iostream>
#include <format>

using namespace std;

struct Person
{
    string name;
    int code;
};

// custom formatter
template<>
struct std::formatter<Person>
{
    constexpr auto parse(std::format_parse_context& ctx) 
    {
        return ctx.begin(); // return position of } (hopefully there - this is a toy example)
    }
    
    auto format(const Person& p, std::format_context& ctx) const 
    {
        return std::format_to(ctx.out(), "{} {}", p.name, p.code);
    }
};

int main()
{
    // format to strings
    cout << format("Finally we can format strings: {} {}\n", 42, "hello");
    
    // format to output iterators
    format_to(ostream_iterator<char>{cout, ""}, "Format to cout {:.2f}\n", 23.999);
    
    // format to output iterators with at most N characters
    char someArray[20];
    const auto res = format_to_n(someArray, size(someArray)-1, "This string is too long for that buffer {}", 56);
    *res.out = '\0'; // we have to do this manually, if needed
    // res.size is the number of characters that would have been written without truncating the string
    
    Person p { "marco", 5 };
    cout << format("{}", p);
}
