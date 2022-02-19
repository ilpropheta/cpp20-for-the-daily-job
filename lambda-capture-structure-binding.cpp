// C++20 for the daily job (Marco Arena)
//
// CAPTURING STRUCTURE BINDINGS INTO LAMBDA EXPRESSIONS

#include <iostream>
#include <array>

using namespace std;

int main()
{
    array arr = {1,2,3};
    auto& [a1, a2, a3] = arr;
    
    // only a1 is copied, not the whole object
    auto fn = [a1]{
        cout << a1;
    };
    
    fn();
}
