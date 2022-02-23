// C++20 for the daily job (Marco Arena)
//
// LAMBDA CAPTURE REFINEMENTS

#include <iostream>
#include <array>

using namespace std;

template<typename... Args>
void print_all(const Args&... args)
{
    ((std::cout << args << ' '), ...);
}

template<typename... Args>
auto capture_init_this(Args... args) {
    return [...args=move(args)]() { 
        print_all(args...);
    };
}

int main()
{
    // structure binding identifiers capture
    array arr = {1,2,3};
    auto& [a1, a2, a3] = arr;
    // only a1 is copied, not the whole object
    auto fn = [a1]{
        cout << a1 << "\n";
    };
    fn();
    
    // pack expansion in lambda init-capture
    capture_init_this(1,2,3)();
}
