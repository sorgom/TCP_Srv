#include "TCP_Srv_Echo.h"
#include "Trace.h"

#ifndef _WIN32
#include <sys/socket.h>  
#endif

#include <iostream>
using std::cerr, std::endl;
#include <iomanip>
using std::setw;
#include <clocale>

TCP_Srv_Echo& TCP_Srv_Echo::instance()
{
    static TCP_Srv_Echo instance;
    return instance;
}

bool TCP_Srv_Echo::handleArg(const CONST_C_STRING argv)
{
    bool ok = true;
    //  locale only required if trace is on
    if constexpr (Trace::isOn)
    {
        if (std::setlocale(LC_ALL, argv) == nullptr)
        {
            cerr << "ERR setlocale: " << argv << endl;
            ok = false;
        }
        else Trace() << "locale : " << argv << endl;
    }
    return ok;
}

void TCP_Srv_Echo::addUsage() const
{
    Trace() << " [locale]";
}

void TCP_Srv_Echo::addHelp() const
{
    Trace() << "locale: see setlocale" << endl;
}

void TCP_Srv_Echo::process(const SOCKET clientSocket, Buffer buff, const size_t size, const UINT32 nr)
{
    send(clientSocket, buff, size, 0);
 
    if constexpr (Trace::isOn)
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

void TCP_Srv_Echo::otherTasks()
{
    if constexpr (Trace::isOn)
    {
        static UINT32 cnt = 0;
        static UINT32 dsp = 0;
        if (++cnt == 100)
        {
            cnt = 0; 
            ++dsp;
            TraceLock() << "--- other tasks " << setw(5) << dsp << "00" << endl;
        }
    }
}