#include "TCP_Srv_Echo.h"
#include <clocale>
#include <string>
#include <iostream>
int main(const INT32 argc, const CONST_C_STRING* const argv)
{
    std::setlocale(LC_ALL, "de_DE.UTF-8");
    TCP_Srv_Echo srv;
    srv.run(argc, argv);
    return 0;
}