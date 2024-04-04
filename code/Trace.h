//  ============================================================
//  Tracing dependent on VERBOSE compile define
//  intended to avoid macro usage
//  ============================================================
//  created by Manfred Sorgo

#pragma once
#ifndef TRACE_H
#define TRACE_H

#include "BaseTypes.h"
#include <ciso646>

#include <iostream>
#include <iomanip>
#include <mutex>

//  class Trace blocks (std::cout) output within object scope if VERBOSE is not defined
//  if VERBOSE is defined, Trace behaves like std::cout
class Trace
{
public:
//  avoid macro usage
//  use:
//      constexpr if (Trace::verbose) { ... }
//      constexpr if (not Trace::verbose) { ... }
//  to eliminate code from compilation
//  in opposite to macros there won't be compiler warnings of unused variables
#ifdef VERBOSE
    constexpr static bool verbose = true;
#else
    constexpr static bool verbose = false;
#endif
    Trace()
    {
        if constexpr (not verbose)
        {
            std::cout.setstate(std::ios_base::badbit);
        }
    }
    ~Trace()
    {
        if constexpr (not verbose)
        {
            std::cout.clear();
        }
    }
    template <typename T>
    inline std::ostream& operator<<(const T& t)
    {
        return std::cout << t;
    }
};

//  class TraceLock behaves like class Trace
//  in addition, if VERBOSE is defined
//  parallel output (of other TraceLock objects)
//  is blocked by a mutex lock within object scope
class TraceLock : public Trace
{
public:
#ifdef VERBOSE
    TraceLock() : Trace(), mLock(mMutex) 
    {}
    TraceLock(const UINT32 nr) : TraceLock() 
    {
        *this << std::setw(3) << nr << ' '; 
    }
private:
    static std::mutex mMutex;
    std::unique_lock<std::mutex> mLock;    
#else
    TraceLock(const UINT32 = 0) {}
#endif
};

#endif // _H
