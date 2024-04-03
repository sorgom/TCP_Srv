#include "TCP_Srv_Echo.h"

int main(const INT32 argc, const CONST_C_STRING* const argv)
{
    TCP_Srv_Echo srv;
    srv.run(argc, argv);
    return 0;
}