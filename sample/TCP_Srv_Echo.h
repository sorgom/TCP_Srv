
#pragma once
#ifndef TCP_SRV_ECHO_H 
#define TCP_SRV_ECHO_H

#include <TCP_Srv/TCP_Srv_Base.h>
#include <mutex>

class TCP_Srv_Echo : public TCP_Srv_Base
{
public:
    inline TCP_Srv_Echo() = default;
protected:   
    void process(const SOCKET clientSocket, Buffer buff, size_t n) final;
private:
    //  prevent from parallel output
    std::mutex mMtx;
};

#endif // _H
