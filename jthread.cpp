// C++20 for the daily job (Marco Arena)
//
// std::jthread and cooperative cancellation

#include <iostream>
#include <thread>
#include <vector>
#include <syncstream>
#include <mutex>
#include <queue>
#include <condition_variable>

using namespace std;
using namespace std::literals;

int main()
{
    // jthread hello world
    {
        jthread t1{[]{
            cout << "hello jthread\n\n";
        }};
        
        // t1.join() is called in jthread's destructor
    }
    
    // jthread with stop_token and stop_callback
    {    
        jthread t2{[](stop_token st) {        
            this_thread::sleep_for(1ms);
            stop_callback sc1{st, []{
                cout << "stop requested 1. Executed on thread: " << this_thread::get_id() << "\n";;
            }};
            stop_callback sc2{st, []{
                cout << "stop requested 2. Executed on thread: " << this_thread::get_id() << "\n";;
            }};
        }};
        
        std::cout << "main thread is: " << this_thread::get_id() << "\n";
        
        // jthread's destructor calls request_stop() and then join()
    }
    
    cout << "\n";
    
    // jthread with stop_token and stop_callback - stop_callbacks *probably* executed on main thread
    {    
        jthread t2{[](stop_token st) {                    
            stop_callback sc1{st, []{
                cout << "another stop requested 1. Executed on thread: " << this_thread::get_id() << "\n";;
            }};
            stop_callback sc2{st, []{
                cout << "another stop requested 2. Executed on thread: " << this_thread::get_id() << "\n";;
            }};
            this_thread::sleep_for(5ms);            
        }};
        
        this_thread::sleep_for(2ms);
        std::cout << "main thread is: " << this_thread::get_id() << "\n";
        
        // jthread's destructor calls request_stop() and then join()
    }
    
    // in this case, the thread will likely finish before calling jthread's destructor
    // and then the callbacks will be destructed (aka: deregistered) before the stop request arrives
    {   
        jthread t3{[](stop_token st) {        
            stop_callback sc1{st, []{
                cout << "probably, I won't be called 1\n";
            }};
            stop_callback sc2{st, []{
                cout << "probably, I won't be called 2\n";
            }};
        }};
        this_thread::sleep_for(10ms);
    }
     
    // here we just pass our own stop_token to a bunch of jthreads
    {
        cout << "\n";
        stop_source src;
        auto token = src.get_token(); // only one token to rule them all
        vector<jthread> workers;
        for (auto i=0; i<5; ++i)
        {
            workers.emplace_back([=](stop_token s){
                auto counter = 0;
                while (!s.stop_requested())
                {
                    ++counter;
                    this_thread::sleep_for(chrono::milliseconds(3+i));
                }
                osyncstream(cout) << "Worker " << i << " has been stopped...counter=" << counter << "\n";
            }, token);
        }
        
        this_thread::sleep_for(100ms);
        cout << "stopping workers...\n";
        src.request_stop();
    }
    
    // what's the difference between the above and this below?
    {
        cout << "\n";
        vector<jthread> workers;
        for (auto i=0; i<5; ++i)
        {
            workers.emplace_back([=](stop_token s){
                auto counter = 0;
                while (!s.stop_requested())
                {
                    ++counter;
                    this_thread::sleep_for(chrono::milliseconds(3+i));
                }
                osyncstream(cout) << "Worker " << i << " has been stopped...counter=" << counter << "\n";
            });
        }
        this_thread::sleep_for(100ms);
    }
    
    // using condition_variable_any with stop_token
    {
        cout << "\n";
        condition_variable_any cv;
        mutex m;
        bool sent = false;
        
        jthread ping { [&]{
            std::this_thread::sleep_for(5ms);
            {
                std::lock_guard l{m};
                sent = true;
                std::cout << "PING sent...\n";
            }
            cv.notify_one();
        }};
        
        jthread pong { [&](stop_token st){
            std::unique_lock l{m};
            if (cv.wait(l, st, [&]{ return sent; }))
            {
                std::cout << "PING received: " << '\n';
            }
            else
            {
                std::cout << "Stop received" << '\n';
            }
        }};
        
        this_thread::sleep_for(1ms);
        
        // see another example here: https://www.nextptr.com/tutorial/ta1588653702/stdjthread-and-cooperative-cancellation-with-stop-token
    }
}
