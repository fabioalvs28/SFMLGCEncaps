#ifndef DEFINE_H
#define DEFINE_H

#include <iostream>
#include <cassert>

#ifdef NDEBUG
#define ASSERT(condition, message) ((void)0)
#else
#define ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            std::cerr << "Assertion failed: (" << #condition << "), function " << __FUNCTION__ \
                      << ", file " << __FILE__ << ", line " << __LINE__ << ".\n" << "Message: " << message << std::endl; \
            std::terminate(); \
        } \
    } while (false)
#endif

//Check Memory leak 
#ifdef _MSC_VER
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

/// <summary>
/// Call this function at entry point
/// </summary>
inline void EnableMemoryLeakDetection()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
}

#define ENABLE_MEMORY_LEAK_DETECTION() EnableMemoryLeakDetection()

#else
#define ENABLE_MEMORY_LEAK_DETECTION() ((void)0)
#endif

#endif