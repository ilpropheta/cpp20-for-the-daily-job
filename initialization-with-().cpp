// C++20 for the daily job (Marco Arena)
//
// INITIALIZATION WITH ( )

#include <iostream>
#include <string>
#include <memory>

using namespace std;

struct Person
{
    std::string name;
    int code;
    bool flag = true;
};

int main()
{
    auto p1 = make_unique<Person>("marco", 15);
    auto p2 = make_unique<Person>("marco", 15, false);
    
    int arr[3](1,2);
    cout << "array initialized with (): " << arr[0] << " " << arr[1] << " " << arr[2] << "\n";
    
    struct ElideThis
    {
        int x;
        struct Sub
        {
            int a;
            std::string b;
        } subj;
    };
    
    ElideThis obj {10, 20, "hello"};
    //ElideThis obj (10, 20, "hello"); // ill-formed because direct-initialization does not allow brace elision
            
    struct B
    {
        int a;
        std::string&& r;
    };
    
    B b1{10, "ABCDEFGHILMNOPQRSTUVZ"}; // ok
    B b2(10, "ABCDEFGHILMNOPQRSTUVZ"); // r is a dangling reference...
    cout << "Expecting ABCDEFGHILMNOPQRSTUVZ: " << b1.r << "\n";
    cout << "Expecting ABCDEFGHILMNOPQRSTUVZ: " << b2.r << "\n"; // well, b2.r is dangling...    
}
