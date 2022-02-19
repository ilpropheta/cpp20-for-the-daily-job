// C++20 for the daily job (Marco Arena)
//
// INITIALIZATION WITH ( ) AND  { }

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
}
