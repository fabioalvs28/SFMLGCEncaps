#ifndef DEFINE_H
#define DEFINE_H
#define PI 3.1415927f

#include <iostream>
#include <cassert>

enum LogLevel{
    LOG_WARNING,
    LOG_FATAL
};

//Use Example:
// int x = 5;
// ASSERT( x == 5, LOG_WARNING, "x should be 5");
// ASSERT( x == 5, LOG_FATAL, "x should be 5");
#ifdef NDEBUG
#define ASSERT(condition, level, message) ((void)0)
#else
#define ASSERT(condition, level, message) \
    do { \
        if (!(condition)) { \
            std::cerr << "Assertion failed: (" << #condition << "), function " << __FUNCTION__ \
                      << ", file " << __FILE__ << ", line " << __LINE__ << ".\n" << "Message: " << message << std::endl; \
            if(level == LOG_FATAL){ \
                ifstd::terminate(); \
            } \
            else if(level == LOG_WARNING){ \
                std::cerr << "Warning: " << message <<std::endl; \
            } \
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