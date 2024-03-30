//  ============================================================
//  TCP server base class
//  ============================================================
//  created by Manfred Sorgo

#pragma once
#ifndef TCP_SRV_BASE_H
#define TCP_SRV_BASE_H

#include <TCP_Srv/BaseTypes.h>

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32")
using tval = const timeval;
#else
#include <arpa/inet.h>
#include <sys/socket.h>  
#include <unistd.h>
using SOCKET = int;
constexpr SOCKET INVALID_SOCKET = -1;
#define closesocket close
using tval = timeval;
#endif

#include <stdlib.h> // atoi
#include <mutex>

#ifndef SELECT_SECONDS
#define SELECT_SECONDS 0
#endif

#ifndef SELECT_MICRO_SECONDS
#define SELECT_MICRO_SECONDS 10
#endif

#ifndef READ_BUFFER_SIZE
#define READ_BUFFER_SIZE 1024
#endif

class TCP_Srv_Base
{
public:
    inline TCP_Srv_Base() = default;
    bool run(UINT16 port = 8080);
    bool run(const INT32 argc, const CONST_C_STRING* const argv);

protected:
    using Buffer = CHAR[READ_BUFFER_SIZE];
    virtual void process(SOCKET cs, Buffer buff, size_t n) = 0;
    const timeval mSelectTime = {SELECT_SECONDS, SELECT_MICRO_SECONDS};

private:
    // thread method
    void tm(SOCKET cs);
    bool cleanup();
    SOCKET mListenSocket = INVALID_SOCKET;
};

#endif // _H