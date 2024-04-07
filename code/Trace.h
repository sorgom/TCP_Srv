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
//  thanks to ChatGPT for the basics of this
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
    constexpr static bool isOn = false;
    inline NullStream(UINT32 = 0) : std::ostream(&mBuffer) {}
private:
    NullBuffer mBuffer;
};

//  ostream class acting as std::cout
//  thanks to ChatGPT for the basics of this
class OutStream : public std::ostream
{
public:
    constexpr static bool isOn = true;
    inline OutStream() : std::ostream(std::cout.rdbuf()) {}
};

//  class LockedStream behaves like class OutStream
//  in addition, parallel output (of other LockedStream objects)
//  is blocked by a mutex lock within object scope
//  use brackets to limit scope of LockedStream object
//  to avoid deadlocks
//  e.g. 
//  { LockedStream() << "hello world" << std::endl; }
class LockedStream : public OutStream
{
public:
    LockedStream() : OutStream(), mLock(mMutex) {}
    //  since threading is involved, a (thread) number can be added to the output
    LockedStream(const UINT32 nr) : LockedStream()
    {
        *this << std::setw(3) << nr << ' ';
    }
private:
    static std::mutex mMutex;
    std::unique_lock<std::mutex> mLock;
};

//  if VERBOSE is defined
//  -  class Trace is OutStream
//  -  class TraceLock is LockedStream
//  -  class Whisper is NullStream  
//  otherwise
//  -  class Trace is NullStream
//  -  class TraceLock is NullStream
//  -  class Whisper is OutStream  
//  avoid macro usage
//  use:
//      constexpr if (Trace::verbose) { ... }
//      constexpr if (not Trace::verbose) { ... }
//  to eliminate greater bits of output code from compilation
//  in opposite to macros there won't be compiler warnings of unused variables
#ifdef VERBOSE
using Trace = OutStream;
using TraceLock = LockedStream;    
using Whisper = NullStream;
#else
using Trace = NullStream;
using TraceLock = NullStream;
using Whisper = OutStream;
#endif

extern Trace trace;
extern Whisper whisper;

#endif // _H
