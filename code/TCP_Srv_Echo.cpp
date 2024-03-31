#include "TCP_Srv_Echo.h"

#include <iostream>
using std::cout;
using std::endl;
#include <iomanip>

void TCP_Srv_Echo::process(const SOCKET clientSocket, Buffer buff, const size_t size, const UINT32 nr)
{
    send(clientSocket, buff, size, 0);
#ifdef TRACE_ON    
    mutexlock lock(mMtxOut);
    cout << std::setw(3) << nr << " <- ";
    for (size_t n = 0; n < size; ++n)
    {
        cout << buff[n];
    }
    cout << " ->" << endl;
#endif
}
