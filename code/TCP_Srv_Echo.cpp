#include "TCP_Srv_Echo.h"

#include <iostream>
using std::cout, std::cerr, std::endl;
#include <iomanip>

#include <clocale>

bool TCP_Srv_Echo::handlearg(const CONST_C_STRING argv)
{
    if (std::setlocale(LC_ALL, argv) == nullptr)
    {
        cerr << "setlocale failed: " << argv << endl;
        return false;
    }
#ifdef VERBOSE                    
    else
    {
        cout << "locale : " << argv << endl;
    }
#endif
    return true;
}

void TCP_Srv_Echo::help(const std::string&& argv0) const
{
    cout 
        << endl
        << "usage : " << argv0 << " [-h] [port] [locale]" << endl
        << "-h    : this help" << endl
        << "port  : 2-5 digits, default: " << defPort << endl
        << "locale: see setlocale" << endl;
}

void TCP_Srv_Echo::process(const SOCKET clientSocket, Buffer buff, const size_t size, const UINT32 nr)
{
    send(clientSocket, buff, size, 0);
#ifdef VERBOSE    
    mutexlock lock(mMtxOut);
    cout << std::setw(3) << nr << " <- ";
    for (size_t n = 0; n < size; ++n)
    {
        cout << buff[n];
    }
    cout << " ->" << endl;
#endif
}
