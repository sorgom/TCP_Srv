
#include "Trace.h"
#ifdef VERBOSE
std::mutex TraceLock::mMutex;
#else
NullStream Trace::mNull;
#endif // VERBOSE