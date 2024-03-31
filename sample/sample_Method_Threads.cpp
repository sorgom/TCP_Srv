//  ============================================================
//  sample of threads with object methods
//  ============================================================
//  created by Manfred Sorgo

//  header content
#include <mutex>

class Test
{
public:
    inline Test(int num = 0) : 
        mOff(num), 
        mRunning(false)
    {}
    void start();
    void stop();
private:
    // thread method
    void tm(int s);
    // context number offset
    const int mOff;
    // thread synchronization
    std::mutex mMtx;
    // thread control
    bool mRunning;
};

//  source content
#include <thread>
#include <iostream>
using std::cout;
using std::endl;
using std::this_thread::sleep_for;
using std::chrono::milliseconds;
using std::chrono::seconds;
using mutextlock = std::unique_lock<std::mutex>;

void Test::start()
{
    cout << "start" << endl;
    mRunning = true; 
    for (int i = 0; i < 10; ++i)
    {
        //  this is the way to start a thread with a class method
        //  1st parameter: class::method
        //  2nd parameter: object -> this
        //  following parameters: method parameters
        std::thread t(&Test::tm, this, i);
        t.detach();
    }
}

void Test::stop() 
{
    mutextlock lock(mMtx);
    cout << "stop ..." << endl; 
    mRunning = false;
    lock.unlock(); 
    sleep_for(milliseconds(300));
}

void Test::tm(int s)
{
    mutextlock lock(mMtx);
    lock.unlock();
    while (mRunning)
    {
        sleep_for(milliseconds(100));
        lock.lock();
        cout << "tm " << s << ": " << mOff + s << endl;
        lock.unlock();
    }
    lock.lock();
    cout << "tm " << s << " done." << endl; 
}

//  main content
int main()
{
    Test tt(10);
    tt.start();
    sleep_for(seconds(1));
    tt.stop();
    cout << "test done." << endl;
    return 0;
}