#pragma warning(disable : 4996)
#define WINVER 0x0502
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <tchar.h>

using namespace std;

#define BUFSIZE MAX_PATH
#define FILESYSNAMEBUFSIZE MAX_PATH
#define INFO_BUFFER_SIZE 32767
#define MAX_KEY_LENGTH 255

//2.
double LenFreq = 0.0;
__int64 CountStart = 0;

void StartCounter()
{
    LARGE_INTEGER a;
    if (!QueryPerformanceFrequency(&a))
        cout << "Function QueryPerformanceFrequency() failed!\n";

    LenFreq = double(a.QuadPart);

    printf("\n2.1.  CPU frequency: %u  Hz\n", a);
    QueryPerformanceCounter(&a);
    CountStart = a.QuadPart;
}

int main()
{
     // Пункт 1.1
    OSVERSIONINFO opsys = {0};
    opsys.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&opsys);
    printf("1.1.  OS: Windows v%d.%d  (build %d)", opsys.dwMajorVersion, opsys.dwMinorVersion, opsys.dwBuildNumber);

    // Пункт 1.2
    TCHAR  Buf[INFO_BUFFER_SIZE];
    DWORD  BChCnt = INFO_BUFFER_SIZE;
    GetSystemDirectory(Buf, INFO_BUFFER_SIZE);
    printf("\n1.2.  System directory: %s", Buf);

    // Пункт 1.3
    BChCnt = INFO_BUFFER_SIZE;
    GetComputerName(Buf, &BChCnt);
    printf("\n1.3.  Computer Name: %s", Buf);
    GetUserName(Buf, &BChCnt);
    printf("\n      User Name: %s", Buf);
    // Пункт 1.4
     char buff[MAX_PATH +1];
    DWORD  CBufLen = MAX_PATH;
    char name[MAX_PATH +1];
    CHAR VolName[MAX_PATH];
    __int64 total, available, free;

    HANDLE search = FindFirstVolume(VolName, BUFSIZE);
    printf("\n      %s", VolName);

    do {
        printf("\n      %s", name);
        GetVolumePathNamesForVolumeName(name, buffer, CBufLen, &CBufLen);
        char* path = buff;
        printf("\n      First path: %s", path);
        GetDiskFreeSpaceEx(buff, (PULARGE_INTEGER)&available, (PULARGE_INTEGER)&total, (PULARGE_INTEGER)&free);
        if (GetDiskFreeSpaceEx(buffer, (PULARGE_INTEGER)&available, (PULARGE_INTEGER)&total, (PULARGE_INTEGER)&free)==0) {
            printf("\n      No information available.\n ");
        }
        else{
        printf("\n      Total size: %u  bytes ", total);
        printf("\n      Free space: %u  bytes\n", available);
        }

    } while (FindNextVolume(search, name, BUFSIZE));
    FindVolumeClose(search);
    // Пункт 1.5
    DWORD dwSize;
    TCHAR szName[MAX_KEY_LENGTH];
    HKEY hKey;
    DWORD dwIndex = 0;
    DWORD retCode;
    DWORD BufferSize = 8192;
    PPERF_DATA_BLOCK PerfData = (PPERF_DATA_BLOCK)malloc(BufferSize);
    DWORD cbData = BufferSize;

    if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run",
        0, KEY_ALL_ACCESS, &hKey) == !ERROR_SUCCESS)
    {
        printf("Function RegOpenKeyEx() failed!\n");
    }
    else printf("\n1.5.  Startup commands:\n");

    while (1) {
        dwSize = sizeof(szName);
        retCode = RegEnumValue(hKey, dwIndex, szName, &dwSize, NULL, NULL, NULL, NULL);
        if (retCode == ERROR_SUCCESS)
        {
            RegQueryValueEx(hKey, szName, NULL, NULL, (LPBYTE)PerfData, &cbData);
            printf("      %d: %s:  %s\n", dwIndex + 1, szName, PerfData);
            dwIndex++;
        }
        else break;
    }

    RegCloseKey(hKey);

    StartCounter();
    cout << "2.2.  CPU clock count: " << GetCounter()  << "  us \n";
    return 0;
}
