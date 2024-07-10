#pragma once
#include <windows.h>
#include <iphlpapi.h>
#include <vector>
#include <string>
#include <iostream>

#pragma comment(lib, "iphlpapi.lib")

class GCSystem
{
public:
    static void GetScreenResolution(int& width, int& height)
    {
        width = GetSystemMetrics(SM_CXSCREEN);
        height = GetSystemMetrics(SM_CYSCREEN);
    }

    static int GetCoreNumber()
    {
        SYSTEM_INFO sysinfo;
        GetSystemInfo(&sysinfo);
        return sysinfo.dwNumberOfProcessors;
    }

    static std::vector<std::string> getMACAddresses()
    {
        std::vector<std::string> macAddresses;
        IP_ADAPTER_INFO adapterInfo[16];
        DWORD bufferSize = sizeof(adapterInfo);

        DWORD status = GetAdaptersInfo(adapterInfo, &bufferSize);
        if (status != ERROR_SUCCESS)
        {
            std::cerr << "Error getting adapter info: " << status << std::endl;
            return macAddresses;
        }

        PIP_ADAPTER_INFO pAdapterInfo = adapterInfo;
        while (pAdapterInfo)
        {
            char macAddress[18];
            snprintf(macAddress, sizeof(macAddress), "%02X:%02X:%02X:%02X:%02X:%02X",
                pAdapterInfo->Address[0], pAdapterInfo->Address[1], pAdapterInfo->Address[2],
                pAdapterInfo->Address[3], pAdapterInfo->Address[4], pAdapterInfo->Address[5]);
            macAddresses.push_back(macAddress);
            pAdapterInfo = pAdapterInfo->Next;
        }

        return macAddresses;
    }
};