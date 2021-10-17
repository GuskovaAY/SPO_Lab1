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
    LARGE_INTEGER CounterStart, PerfFrequency, CounterEnd;

	// замер количества тактов процессора
    QueryPerformanceCounter(&CounterStart);
	// замер частоты счетчика производительности
    QueryPerformanceFrequency(&PerfFrequency);
	// еще раз замерили количество тактов, теперь можем определить сколько тактов ушло на QPF
    QueryPerformanceCounter(&CounterEnd);
    cout << "\nPerformanceFrequency: " << PerfFrequency.QuadPart << " Hz\n";
	// считаем по формуле из лабника
    double result = (1000000.0 * (CounterEnd.QuadPart - CounterStart.QuadPart)) / PerfFrequency.QuadPart;
    cout << "Number of cpu cycles: " << result << " us \n";
}

int main()
{
 // 1.1
	// OSVERSIONINFO - сложный тип данных (структура), содержит информацию о версии
    OSVERSIONINFO os_info = { 0 };
	// задаем в поле структуры размер, который занимает переменная с версией  
    os_info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	// & - извлечение адреса из переменной, значит кладем значение версии по адресу os_info
    GetVersionEx(&os_info);
	// выводим старшую и младшую версии из соответствующих полей структуры
    printf("Version OS: %i.%i\n", os_info.dwMajorVersion, os_info.dwMinorVersion);
 // 1.2
	// создаем символьный массив размером MAX_PATH, в который сохраним путь до системной директории  
    TCHAR buffer[MAX_PATH];
	// сохраняем путь в указанный массив, указываем так же размер этого массива
    GetSystemDirectory(buffer, BUFFER_SIZE);
    printf("\nSystem Directory: %s \n", buffer);
 // 1.3
	// аналогично, записываем имя компьютера и пользователя, но передаем вместо длины массива адрес на переменную, содержащую длину массива
	// DWORD - double word, двойное машинное слово, по сути целочисленная переменная
    DWORD leng = sizeof(buffer);
    GetComputerName(buffer, &leng);
    printf("\nName Computer: %s \n", buffer);
    GetUserName(buffer, &leng);
    printf("Name User: %s \n", buffer);
 // 1.4
    TCHAR buf[MAX_PATH];
    TCHAR name[MAX_PATH];
    DWORD  BufferLength = MAX_PATH;
	// ULARGE_INTEGER - тоже сложный тип
    ULARGE_INTEGER free, volum;
	// HANDLE - дескриптор (переменная, позволяющая пользоваться каким то объектом) ,
	// ищем первый том, в buf записывается UUID тома
    HANDLE search = FindFirstVolume(buf, sizeof(buf));
    do {
		// по UUID получаем букву диска
        GetVolumePathNamesForVolumeName(name, buf, BufferLength, &BufferLength);
        printf("\n\n%s", name);
        printf("\n%s", buf);
		// получаем по букве диска количество свободного места(free) и его объем(volum)
        if (GetDiskFreeSpaceEx(name, &free, &volum, NULL) != 0) {
			// выводим эти значения (.QuadPart - поле структуры ULARGE_INTEGER, ее представление в виде целого числа)
            cout << "\nVolume: " << volum.QuadPart << "\nFree space: " << free.QuadPart;
        }
        else {
            printf("\nNo information.");
        }
	// когда нашли последний том, завершаем цикл
    } while (FindNextVolume(search, name, sizeof(buf)));
    FindVolumeClose(search);
// 1.5
	// ключ реестра
    HKEY hKey;
    DWORD i = 0;
    TCHAR data[BUFFER_SIZE];
    TCHAR vol_name[BUFFER_SIZE];
    DWORD size_vol_name;
    DWORD size_data = BUFFER_SIZE;

    printf("\n\nList of commands:\n");
	// получаем ключ из реестра по указанному адресу, записываем в hKey
    RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_ALL_ACCESS, &hKey);
    do {
        size_vol_name = sizeof(vol_name);
		// имея ключ, получаем доступ к записи в реестре, сохраняем ее в data
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
