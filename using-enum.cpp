// C++20 for the daily job (Marco Arena)
//
// USING ENUMS

#include <iostream>
#include <string>
#include <map>

using namespace std;

enum class LogLevel
{
   trace, debug, info, warning, error, fatal
};

std::string_view ToString(LogLevel level) 
{
   switch(level) 
   {
     using enum LogLevel;
     case trace:   return "trace";
     case debug:   return "debug";
     case info:    return "info";
     case warning: return "warning";
     case error:   return "error";
     case fatal:   return "fatal";
     default: throw std::runtime_error("should never happen");
   }
}

int main()
{
    auto level = LogLevel::trace;
    cout << ToString(level);
}
