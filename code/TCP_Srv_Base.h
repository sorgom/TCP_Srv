//  ============================================================
//  TCP server base class
//  ============================================================
//  created by Manfred Sorgo

#pragma once
#ifndef TCP_SRV_BASE_H
#define TCP_SRV_BASE_H

//  ============================================================
//  behavior settings by compile time defines

//  select timeout in milli seconds
#ifndef SELECT_MILLI_SECONDS
#define SELECT_MILLI_SECONDS 10
#endif

//  buffer size for reading
#ifndef READ_BUFFER_SIZE
#define READ_BUFFER_SIZE 1024
#endif

//  ============================================================

#include "BaseTypes.h"

#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>  
using SOCKET = INT32;
#endif
#include <mutex>
#include <string>

class TCP_Srv_Base
{
public:
    inline TCP_Srv_Base() = default;
    //  run with given port
    void run(UINT16 port = defPort);
    //  run with port from 1st CLI argument
    void run(INT32 argc, const CONST_C_STRING* argv);

protected:
    //  timeval seconds
    constexpr static UINT32 tmSec = SELECT_MILLI_SECONDS / 1000;
    //  timeval microseconds
    constexpr static UINT32 tmMic = (SELECT_MILLI_SECONDS % 1000) * 1000;
    //  (minimum) buffer size
    constexpr static UINT32 buffSize = READ_BUFFER_SIZE < 4 ? 4 : READ_BUFFER_SIZE;
    using Buffer = CHAR[buffSize];
    
    //  process received data
    //  must be implemented by derived class
    virtual void process(const SOCKET clientSocket, Buffer buff, size_t size, UINT32 nr) = 0;

    //  handle unmatched argument
    //  to be implemented by derived class
    inline virtual bool handlearg(CONST_C_STRING argv)
    {
        return true;
    }

    //  display help with called filename (rvalue reference for efficiency)   
    //  to be implemented by derived class
    inline virtual void help(const std::string&& argv0) const {}

    //  prevent from parallel output
    std::mutex mMtxOut;
    using mutexlock = std::unique_lock<std::mutex>;

    //  default port
    constexpr static UINT16 defPort = 8080;

private:
    //  thread method
    void tm(SOCKET clientSocket, UINT32 nr);
    //  thread count
    UINT32 mCnt = 0;
    //  thread number
    UINT32 mNum = 0;
    //  mutex for thread count and number
    std::mutex mMtxStat;
    //  start thread with increased thread number and count
    void startThread(SOCKET clientSocket);
    //  decrease thread count, reset thread number when count is 0    
    void endOfThread();
    //  display current number of threads
    //  (unless VERBOSE is defined)
    void displayThreads() const;

};

#endif // _H