//  ============================================================
//  sample TCP echo server class main
//  ============================================================
//  created by Manfred Sorgo

#include "TCP_Srv_Echo.h"
#include <csignal>
void signalHandler(int)
{
    TCP_Srv_Echo::instance().cleanup();
    exit(0);
}

int main(const INT32 argc, const CONST_C_STRING* const argv)
{
    signal(SIGINT, signalHandler);
    TCP_Srv_Echo::instance().run(argc, argv);
    //  only reached in case of failure
    return 1;
}
