#include <TCP_Srv/TCP_Srv_Base.h>
#include <thread>

#include <iostream>
using std::cout;
using std::endl;

void TCP_Srv_Base::tm(int s)
{
    std::unique_lock<std::mutex> lock(mMtx);
    lock.unlock();
    while (mRunning)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        lock.lock();
        cout << "tm " << s << endl;
        process(s);
        lock.unlock();
    }
    lock.lock();
    cout << "tm " << s << " terminated." << endl; 
}

void TCP_Srv_Base::start()
{
    cout << "start" << endl; 
    mRunning = true;
    for (int i = 0; i < 10; ++i)
    {
        std::thread t(&TCP_Srv_Base::tm, this, i);
        t.detach();
    }
}
void TCP_Srv_Base::stop() 
{
    cout << "stop" << endl; 
    mRunning = false; 
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
}

