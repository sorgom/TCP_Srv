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
#else
#include <sys/socket.h>  
using SOCKET = INT32;
#endif

#ifndef SELECT_SECONDS
#define SELECT_SECONDS 0
#endif

#ifndef SELECT_MILLI_SECONDS
#define SELECT_MILLI_SECONDS 10
#endif

#ifndef READ_BUFFER_SIZE
#define READ_BUFFER_SIZE 1024
#endif

class TCP_Srv_Base
{
public:
    inline TCP_Srv_Base() = default;
    void run(UINT16 port = 8080);
    void run(const INT32 argc, const CONST_C_STRING* const argv);

protected:
    using Buffer = CHAR[READ_BUFFER_SIZE];
    virtual void process(const SOCKET clientSocket, Buffer buff, size_t n) = 0;
 
private:
    // thread method
    void tm(SOCKET clientSocket);

    inline static timeval gettval()
    {
        return {SELECT_SECONDS, SELECT_MILLI_SECONDS * 1000};
    }
};

#endif // _H