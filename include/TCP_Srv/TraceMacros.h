#pragma once
#ifndef TRACE_MACROS_H
#define TRACE_MACROS_H

#ifdef TRACE_ON
#include <iostream>
#define TRACE(MSG) std::cout << MSG << std::endl;
#define TRACEVAR(VAR) std::cout << #VAR << ": " << VAR << std::endl;
#else
#define TRACE(MSG)
#define TRACEVAR(VAR)
#endif

#endif // _H
