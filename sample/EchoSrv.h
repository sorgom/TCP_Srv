
#include <TCP_Srv/TCP_Srv_Base.h>

#pragma once
#ifndef ECHO_SRV_H
#define ECHO_SRV_H

class EchoSrv : public TCP_Srv_Base
{
public:
    inline EchoSrv() = default;
protected:   
    void process(int s) override;
private:
    /* data */
};

#endif // _H