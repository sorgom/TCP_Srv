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
    constexpr static bool isOn = false;
private:
    NullBuffer buffer;
};

//  ostream class acting as std::cout
//  thanks to ChatGPT for this
class OutStream : public std::ostream
{
public:
    inline OutStream() : std::ostream(std::cout.rdbuf()) {}
    constexpr static bool isOn = true;
};

//  class Trace acts as std::cout if VERBOSE is defined
//  otherwise as NullStream with no output
#ifdef VERBOSE
using Trace = OutStream;    
#else
using Trace = NullStream;
#endif

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
