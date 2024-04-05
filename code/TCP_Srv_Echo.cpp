#include "TCP_Srv_Echo.h"
#include "Trace.h"

#ifndef _WIN32
#include <sys/socket.h>  
#endif

#include <iostream>
using std::cerr, std::endl;
#include <clocale>

bool TCP_Srv_Echo::handlearg(const CONST_C_STRING argv)
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

void TCP_Srv_Echo::addusage() const
{
    Trace() << " [locale]";
}

void TCP_Srv_Echo::addhelp() const
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

#include <chrono>
#include <thread>
#include <ctime>
void TCP_Srv_Echo::other_tasks()
{
    { TraceLock() << "    other tasks: " << (std::time(nullptr) % 500) << endl; }
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}