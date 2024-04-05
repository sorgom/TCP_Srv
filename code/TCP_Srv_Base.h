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
    //  run with CLI arguments
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

    //  handle unmatched CLI argument
    //  can be implemented by derived class
    inline virtual bool handlearg(CONST_C_STRING)
    {
        return true;
    }

    //  add usage item to std::out
    //  can be implemented by derived class
    inline virtual void addusage() const {}

    //  add help item to std::out
    //  can be implemented by derived class
    inline virtual void addhelp() const {}

    //  other tasks to be done in main loop
    //  can be implemented by derived class
    inline virtual void other_tasks() {}

private:
    //  default port
    constexpr static UINT16 defPort = 8080;
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

    //  display help with called filename (rvalue reference for efficiency)   
    void help(const std::string&& argv0) const;
};

#endif // _H