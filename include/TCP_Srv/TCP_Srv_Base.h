//  ============================================================
//  TCP server base class
//  ============================================================
//  created by Manfred Sorgo

#ifdef _WIN32
#include <WinSock2.h>
#else
#include <sys/time.h>
using SOCKET = int; // for Linux        
#endif
#include <mutex>

#ifndef SELECT_SECONDS
#define SELECT_SECONDS 0
#endif

#ifndef SELECT_MICRO_SECONDS
#define SELECT_MICRO_SECONDS 10
#endif

class TCP_Srv_Base
{
public:
    inline TCP_Srv_Base() : 
        mRunning(false)
    {}
    void start();
    void stop();
protected:
    virtual void process(int s) = 0;
    const timeval mSelectTime = {SELECT_SECONDS, SELECT_MICRO_SECONDS};
private:
    // thread method
    void tm(int s);
    // thread control
    bool mRunning;
    // thread synchronization
    std::mutex mMtx;
};
