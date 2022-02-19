// C++20 for the daily job (Marco Arena)
//
// Lambdas usability changes

#include <iostream>
#include <set>
#include <string>
#include <strings.h>

using namespace std;

int main()
{
    // not super optimized but ok for this example
    auto ciCompare = [](string_view left, string_view right) { 
       const auto cmp = strncasecmp(left.data(), right.data(), std::min(left.size(), right.size()));
	     return cmp == 0 ? (left.size() < right.size()) : cmp < 0;
    };

    set<string, decltype(ciCompare)> names; // default constructing ciCompare
    names.insert("edward");
    names.insert("jack");
    names.insert("anne");
   
    cout << *names.find("Edward") << "\n";
    
    set<string, decltype(ciCompare)> namesInIta;
    names.insert("edoardo");
    names.insert("giacomo");
    names.insert("anna");
    
    names = namesInIta; // default assignment operator generated for ciCompare
    
    // let's use lambdas in some unevaluated contexts
    
    // how much does a stateless lambda occupy?
    cout << sizeof([]{ cout << "hello"; }) << "\n";
    
    // how much does a lambda capturing a 32-bit int occupy?
    auto number = 10;
    cout << sizeof([number]{ cout << number; }) << "\n";
    
    using T1 = decltype([]{});
    using T2 = decltype([]{});
    static_assert(!is_same_v<T1, T2>, "I don't expect this to happen...");
}
