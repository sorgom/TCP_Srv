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

#include <chrono>

using std::string;

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
        else trace << "locale : " << argv << endl;
    }
    return ok;
}

string TCP_Srv_Echo::usage(const CONST_C_STRING argv0) const
{
    return TCP_Srv_Base::usage(argv0) + " [locale]";
}

string TCP_Srv_Echo::help() const
{
    return TCP_Srv_Base::help() + 
        "locale: e.g. de_DE.UTF-8\n";
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
        using hClock = std::chrono::high_resolution_clock;
        constexpr static auto per = 5000;
        constexpr static auto num = SELECT_MILLI_SECONDS > per ? 1 : per / SELECT_MILLI_SECONDS;
        static auto start = hClock::now();
        static auto cnt = 0;
        if (++cnt == num)
        {
            const auto now = hClock::now();
            const auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
            const auto avrg = (dur.count() + num / 2) / num; 
            TraceLock() << "-   other tasks every " << avrg << " ms" << endl;
            cnt = 0;
            start = now;
        }
    }
}