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
    TCP_Srv_Echo(const TCP_Srv_Echo&) = delete;
    TCP_Srv_Echo& operator=(const TCP_Srv_Echo&) = delete;

protected:
    //  simple implementation of process: 
    //  - echo received data
    //  - trace received data if VERBOSE is defined
    void process(const SOCKET clientSocket, Buffer buff, size_t size, UINT32 nr) final;

    //  handle unused argument as locale to set
    //      does not make a big difference with gcc linux
    //      but with msvc / windows
    bool handleArg(CONST_C_STRING argv) final;

    //  usage line string
    std::string usage(CONST_C_STRING argv0) const final;

    //  help text string
    std::string help() const final;

    //  puts out the average time between calls
    void otherTasks() final;
};

#endif // _H
