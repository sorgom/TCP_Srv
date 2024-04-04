#include "TCP_Srv_Echo.h"
#include "Trace.h"

#ifndef _WIN32
#include <sys/socket.h>  
#endif

#include <iostream>
using std::cout, std::cerr, std::endl;
#include <iomanip>
#include <clocale>

bool TCP_Srv_Echo::handlearg(const CONST_C_STRING argv)
{
    bool ok = true;
    if (std::setlocale(LC_ALL, argv) == nullptr)
    {
        cerr << "ERR setlocale: " << argv << endl;
        ok = false;
    }
    else
    {
        Trace() << "locale : " << argv << endl;
    }
    return ok;
}

void TCP_Srv_Echo::addusage(std::ostream& os) const
{
    os << " [locale]";
}

void TCP_Srv_Echo::addhelp(std::ostream& os) const
{
    os << "locale: see setlocale" << endl;
}

void TCP_Srv_Echo::process(const SOCKET clientSocket, Buffer buff, const size_t size, const UINT32 nr)
{
    send(clientSocket, buff, size, 0);
 
    if constexpr (Trace::verbose)
    {
        TraceLock tr(nr);
        tr << "<- ";
        for (size_t n = 0; n < size; ++n)
        {
            tr << buff[n];
        }
        tr << " ->" << endl;       
    }
}
