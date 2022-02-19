// C++20 for the daily job (Marco Arena)
//
// Other concurrency features

#include <iostream>
#include <atomic>
#include <semaphore>
#include <latch>
#include <barrier>
#include <thread>
#include <vector>
#include <atomic>

using namespace std;
using namespace std::literals;

int main()
{
   constexpr auto numberOfWorkers = 5;

    // simple usage of latch
   { 
        vector<jthread> workers(numberOfWorkers);
        latch done(ssize(workers));
        
        for (auto i=0; auto& w : workers)
        {
            w = jthread{ [&]{
                std::this_thread::sleep_for(chrono::milliseconds(10*(i++)));
                done.count_down();
            }};
        }
        
        cout << "[main] waiting for completion...\n";
        done.wait();
        cout << "[main] waiting done...\n";
   }
   
   cout << "\n";
   
   // simple usage of barrier
   // beware: if any thread is concurrently calling a member function of the barrier during the destruction, the behavior is undefined

   {   
        auto phaseIdx = 0;
        barrier done(numberOfWorkers, [&] noexcept {
            cout << "done phase " << phaseIdx++ << "\n";
        });
        
        vector<jthread> workers(numberOfWorkers);
        
        for (auto i=0; auto& w : workers)
        {
            w = jthread{ [&](stop_token st){
                while (!st.stop_requested())
                {
                    std::this_thread::sleep_for(chrono::milliseconds(10*(i++)));
                    done.arrive_and_wait();
                }
                done.arrive_and_drop();
            }};
        }
                
        cout << "[main] let's see how many phases complete before requesting a stop...\n";
   }
   
   cout << "\n";
   
   // simple usage of semaphores
   {
        binary_semaphore restRoom{false}; // the restroom cannot be used yet
        
        jthread t {[&]{
            cout << "[guest] Com'on...cannot resist...\n";
            restRoom.acquire(); // let me use the restroom
            cout << "[guest] Some unrepeatable sounds...\n";
            this_thread::sleep_for(chrono::milliseconds(20));
            restRoom.release(); // done
            cout << "[guest] left the restroom...\n";
        }};
        
        // I am the cleaning service...let me finish
        // ...
        // 
        cout << "[main] cleaning in progress...\n";
        this_thread::sleep_for(chrono::milliseconds(10));
        
        cout << "[main] ok, the restroom is ready to be used\n";
        restRoom.release();
   }
   
   cout << "\n";
   
   // waiting atomics
   {
        atomic_flag done;
        
        jthread worker {[&]{
            this_thread::sleep_for(chrono::milliseconds(20));
            done.test_and_set();
            done.notify_one();
        }};
        
        cout << "waiting...\n";
        done.wait(done.test());
        cout << "done\n";
   }
}
