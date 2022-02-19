// C++20 for the daily job (Marco Arena)
//
// DESIGNATED INITIALIZERS

#include <iostream>
#include <string>

using namespace std;

struct Person
{
    std::string name;
    int code;
    bool flag = true;
};

struct Point
{
	int x;
	int y;
};

struct Line
{ 
    Point p1;
	Point p2;
};

// enable warnings for more fun

int main()
{
    Person p1 {.name="marco", .code=10, .flag=true}; // ok
    Person p2 {.name="marco", .flag=false}; // ok, code=0
    Person p3 {.name="marco", .code=10}; // nope (out of order)
    Person p4 {.name="marco" }; // ok, code=0, flag=true
    
    // explicit nesting
    Line l { .p1={ .x=10, .y=20 }, .p2={ .x=50, .y=60 } };    
}
