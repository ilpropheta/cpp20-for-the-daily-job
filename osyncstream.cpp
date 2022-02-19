// C++20 for the daily job (Marco Arena)
//
// osyncstream

#include <iostream>
#include <thread>
#include <vector>
#include <iterator>
#include <syncstream>
#include <cmath>

using namespace std;

// Binet's formula (http://mathworld.wolfram.com/BinetsFibonacciNumberFormula.html)
int nth_fibo(int i)
{
  static const auto sqrt5 = sqrt(5);
  static const auto phi = (1 + sqrt5) / 2;
  return static_cast<int>( (pow(phi, i) - (-pow(phi, -i))) / sqrt5);
}

int main()
{
    // use this example to play with osyncstream flush workings:
    
    {   // VERSION 1: we expect that each worker flushes all its work together
        //            since osyncstream instance surrounds the for loop (getting destroyed at the end)
        //
        // a possible output:
        // worker 2 calculates fibo(4)=3
        // worker 2 calculates fibo(5)=5
        // worker 2 calculates fibo(6)=8
        // worker 2 calculates fibo(7)=13 ___ worker 2 terminates here
        // worker 1 calculates fibo(2)=1
        // worker 1 calculates fibo(3)=2  ___ worker 1 terminates here
        // worker 3 calculates fibo(6)=8
        // worker 3 calculates fibo(7)=13
        // worker 3 calculates fibo(8)=21
        // worker 3 calculates fibo(9)=34
        // worker 3 calculates fibo(10)=55
        // worker 3 calculates fibo(11)=89 __ worker 3 terminates here
        vector<jthread> workers;
        for (auto worker=0; worker<4; ++worker)
        {
            workers.emplace_back([worker]{
                osyncstream scout{cout};
                for (auto i=worker*2; i<worker*4; ++i)
                {
                    scout << "worker " << worker << " calculates fibo(" << i << ")=" << nth_fibo(i) << "\n";
                }
            });
        }    
    }
    
    cout << "\nVERSION 2\n\n";
    
    {   // VERSION 2: each worker may interleave with the others
        //            since osyncstream instance is temporary in each print
        //
        // a possible output:
        // worker 1 calculates fibo(2)=1
        // worker 1 calculates fibo(3)=2
        // worker 2 calculates fibo(4)=3
        // worker 2 calculates fibo(5)=5
        // worker 2 calculates fibo(6)=8
        // worker 3 calculates fibo(6)=8
        // worker 2 calculates fibo(7)=13  <-- is between worker 3
        // worker 3 calculates fibo(7)=13
        // worker 3 calculates fibo(8)=21
        // worker 3 calculates fibo(9)=34
        // worker 3 calculates fibo(10)=55
        // worker 3 calculates fibo(11)=89
        
        vector<jthread> workers;
        for (auto worker=0; worker<4; ++worker)
        {
            workers.emplace_back([worker]{                
                for (auto i=worker*2; i<worker*4; ++i)
                {
                    osyncstream{cout} << "worker " << worker << " calculates fibo(" << i << ")=" << nth_fibo(i) << "\n";
                }
            });
        }    
    }
    
    cout << "\nVERSION 3\n\n";
    
    // do it yourself: 
    
    // VERSION 3: mix 1 and 2, that is 
    {
        vector<jthread> workers;
        for (auto worker=0; worker<4; ++worker)
        {
            workers.emplace_back([worker]{
                osyncstream scout{cout};
                for (auto i=worker*2; i<worker*4; ++i)
                {
                    scout << "worker " << worker << " calculates fibo(" << i << ")=" << nth_fibo(i) << "\n" << flush_emit;
                }
            });
        }   
    }
}
