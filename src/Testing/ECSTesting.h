#pragma once

#include "Engine.h"

class ECSTesting
{
public:
    static ECSTesting& GetInstance()
    {
        static ECSTesting instance;
        return instance;
    }

    void RunTests();

private:
    /*Constructor and Destructor should not be called
     Use init or cleanup custom methods instead*/
    ECSTesting() = default;
    ~ECSTesting() = default; 
};