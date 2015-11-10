// SetPowerScheme.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <PowrProf.h>

bool CheckGuid(const GUID& guid)
{
    UCHAR buffer[1024];
    ULONG i = 0;
    while (true)
    {
        DWORD bufferSize = sizeof(buffer);
        DWORD retVal = PowerEnumerate(NULL, NULL, NULL, ACCESS_SCHEME, i, buffer, &bufferSize);
        if (retVal != ERROR_SUCCESS)
        {
            if (retVal == ERROR_NO_MORE_ITEMS)
            {
                break;
            }
            else
            {
                printf("PowerEnumerate failed\n");
                exit(-1);
            }
        }

        if (IsEqualGUID(*(GUID*)buffer, guid))
        {
            return true;
        }
        i++;
    }

    return false;
}


int main()
{
    GUID* activePolicyGuid;

    if (PowerGetActiveScheme(NULL, &activePolicyGuid) != ERROR_SUCCESS)
    {
        printf("Cannot get active scheme\n");
        return -1;
    }

    WCHAR s[1024];
    if (!StringFromGUID2(*activePolicyGuid, s, 1024))
    {
        printf("StringFromGUID2 failed\n");
        return -1;
    }

    wprintf(L"Current Power Scheme: %s\n", s);

    if (!IsEqualGUID(GUID_MIN_POWER_SAVINGS, *activePolicyGuid))
    {
        if (!CheckGuid(GUID_MIN_POWER_SAVINGS))
        {
            printf("High Performance Power Scheme not found\n");
            return 1;
        }
        else if (PowerSetActiveScheme(NULL, &GUID_MIN_POWER_SAVINGS) != ERROR_SUCCESS)
        {
            printf("PowerSetActiveScheme failed\n");
            return -1;
        }

        printf("Power Scheme changed to High Performance\n");
    }
    return 0;
}
