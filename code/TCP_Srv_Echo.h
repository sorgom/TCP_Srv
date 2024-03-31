//  ============================================================
//  sample TCP echo server class
//  ============================================================
//  created by Manfred Sorgo

#pragma once
#ifndef TCP_SRV_ECHO_H 
#define TCP_SRV_ECHO_H

#include "TCP_Srv_Base.h"

class TCP_Srv_Echo : public TCP_Srv_Base
{
public:
    inline TCP_Srv_Echo() = default;
protected:
    //  simple implementation of process: echo received data
    //  trace received data if TRACE_ON is defined
    void process(const SOCKET clientSocket, Buffer buff, size_t size, UINT32 nr) final;
};

#endif // _H
