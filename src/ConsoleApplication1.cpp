import <Windows.h>;
import Inject;

#include <iostream>
#include <TlHelp32.h>

//This just gets the process ID
DWORD GetProcID(const wchar_t* processName)
{
    auto handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, TH32CS_SNAPALL);
    DWORD ID = 0;
    if (handle)
    {
        PROCESSENTRY32W entry;
        entry.dwSize = sizeof(entry);

        if (Process32FirstW(handle, &entry))
        {
            do
            {
                if (wcscmp(processName, entry.szExeFile) == 0)
                {
                    ID = entry.th32ProcessID;
                    break;
                }
                
            } while (Process32NextW(handle, &entry));

        }

     }
    return ID;
}

//This is for learning purposes only!
int main()
{
    Inject inj(LR"(C:\Path\To\Dll.dll)", GetProcID(L"example.exe"));

    inj.InjectIntoProcess();
    return 0;
}

