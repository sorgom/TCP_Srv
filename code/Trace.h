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

#include <streambuf>

//  NullBuffer and NullStream are used to prevent from output
//  thanks to ChatGPT for this
class NullBuffer : public std::streambuf 
{
public:
    inline int overflow(int c) override
    {
        return c; // Do nothing with the character
    }
};

class NullStream : public std::ostream
{
public:
    inline NullStream() : std::ostream(&buffer) {}

private:
    NullBuffer buffer;
};

//  class Trace acts as std::cout if VERBOSE is defined
//  otherwise as NullStream with no output
class Trace
{
public:
//  avoid macro usage
//  use:
//      constexpr if (Trace::isOn) { ... }
//      constexpr if (not Trace::isOn) { ... }
//  to eliminate code from compilation
//  in opposite to macros there won't be compiler warnings of unused variables
#ifdef VERBOSE
    constexpr static bool isOn = true;
#else
    constexpr static bool isOn = false;
#endif
    inline Trace() = default;

#ifdef VERBOSE
    template <typename T>
    inline std::ostream& operator<<(const T& t)
    {
        return std::cout << t;
    }
#else
    template <typename T>
    inline std::ostream& operator<<(const T& t)
    {
        return mNull << t;
    }
private:
    static NullStream mNull;
#endif
};

//  class TraceLock behaves like class Trace
//  in addition, if VERBOSE is defined
//  parallel output (of other TraceLock objects)
//  is blocked by a mutex lock within object scope
//  use brackets to limit scope of TraceLock object
//  to avoid deadlocks
//  e.g. 
//  { TraceLock() << "hello world" << std::endl; }
class TraceLock : public Trace
{
public:
#ifdef VERBOSE
    TraceLock() : Trace(), mLock(mMutex) 
    {}
    //  start trace with number
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
