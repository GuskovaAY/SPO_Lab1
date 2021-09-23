#define WINVER 0x0502
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
using namespace std;

#define  BUFFER_SIZE 32767
//2
unsigned long flagf = 0;
__int64 CounterStart = 0;

void Starting()
{
    LARGE_INTEGER t;


    if (!QueryPerformanceFrequency(&t))
        cout << "Function QueryPerformanceFrequency() failed!\n";
    flagf = long(t.QuadPart);
    cout << "\nCPU frequency: " << flagf << " Hz\n";
    QueryPerformanceCounter(&t);
    CounterStart = t.QuadPart;


    if (!QueryPerformanceCounter(&t)) 
        cout << "Function QueryPerformanceCounter() failed!\n";
    else 
    {
        double result = (1000000 * (t.QuadPart - CounterStart)) / flagf;
        cout << "Number of cpu cycles: " << result << " us \n";
    }
}

int main()
{
 // 1.1 
    OSVERSIONINFO os_info = { 0 };
    os_info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&os_info);
    printf("Version OS: %i.%i\n", os_info.dwMajorVersion, os_info.dwMinorVersion)
 // 1.2
    TCHAR buffer[MAX_PATH];
    GetSystemDirectory(buffer, BUFFER_SIZE);
    printf("\nSystem Directory: %s \n", buffer);
 // 1.3
    DWORD leng = sizeof(buffer);
    GetComputerName(buffer, &leng);
    printf("\nName Computer: %s \n", buffer);
    GetUserName(buffer, &leng);
    printf("Name User: %s \n", buffer);
 // 1.4 
    TCHAR buf[MAX_PATH];
    TCHAR name[MAX_PATH];
    DWORD  BufferLength = MAX_PATH;
    ULARGE_INTEGER free, volum;
    HANDLE search = FindFirstVolume(buf, sizeof(buf));
    do {
        GetVolumePathNamesForVolumeName(name, buf, BufferLength, &BufferLength);
        printf("\n\n%s", name);
        printf("\n%s", buf);
        if (GetDiskFreeSpaceEx(name, &free, &volum, NULL) != 0) {
            cout << "\nVolume: " << volum.QuadPart << "\nFree space: " << free.QuadPart;
        }
        else {
            printf("\nNo information.");
        }
    } while (FindNextVolume(search, name, sizeof(buf)));
    FindVolumeClose(search);
// 1.5
    HKEY hKey;
    DWORD i = 0;
    TCHAR vol_name[BUFFER_SIZE];
    TCHAR data[BUFFER_SIZE];
    TCHAR vol_name[BUFFER_SIZE];
    DWORD size_vol_name;
    DWORD size_data = BUFFER_SIZE;

    printf("\n\nList of commands:\n");
    RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_ALL_ACCESS, &hKey);
    do {
        size_vol_name = sizeof(vol_name);
        if (RegEnumValue(hKey, i, vol_name, &size_vol_name, NULL, REG_NONE, NULL, NULL) == ERROR_SUCCESS) {
            RegQueryValueEx(hKey, vol_name, NULL, REG_NONE, (LPBYTE)data, &size_data);
            printf("%i) %s:  %s\n", i + 1, vol_name, data);
            i++;
        }
        else break;
    } while (1);
    RegCloseKey(hKey);

    Starting();

    return 0;
}
