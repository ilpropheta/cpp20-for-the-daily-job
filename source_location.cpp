// C++20 for the daily job (Marco Arena)
//
// std::source_location

#include <iostream>
#include <source_location>

using namespace std;

template <typename T> 
void fun(T)
{
    cout << "This is line number: " << source_location::current().function_name() << "\n";
}

int main()
{
    cout << "This is line number: " << source_location::current().line() << "\n";
    cout << "This is function name: " << source_location::current().function_name() << "\n";
    fun(10);
}
