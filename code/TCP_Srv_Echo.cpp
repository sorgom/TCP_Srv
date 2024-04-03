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
    if constexpr (verbose)
    {
        cout << "locale : " << argv << endl;
    }
    return true;
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
 
    if constexpr (verbose)
    {
        mutexlock lock(mMtxOut);
        cout << std::setw(3) << nr << " <- ";
        for (size_t n = 0; n < size; ++n)
        {
            cout << buff[n];
        }
        cout << " ->" << endl;       
    }
}
