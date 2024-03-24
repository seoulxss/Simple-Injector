module;
//Module fragement
//Put all your includes here!
#include <Windows.h>
module Inject;

//Other imports go here!
import <string>;

//constructor
Inject::Inject(std::wstring_view dllPath, DWORD ProcessID) : m_DllPath(dllPath), m_ProcessID(ProcessID)
{
}

bool Inject::InjectIntoProcess()
{
    //Init everything
    if (!Setup())
        return false;

    // Allocate memory in the remote process
    auto alloc = VirtualAllocEx(m_ProcessHandle, nullptr, 0x1000, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!alloc)
        return false;

    // Write the DLL path into the remote process
    // +1 because of Null-Terminator
    //* sizeof(wchar_t), as wchar_t takes 2 bytes instead of 1
    if (!WriteProcessMemory(m_ProcessHandle, alloc, m_DllPath.data(), (wcslen(m_DllPath.data()) + 1) * sizeof(wchar_t), nullptr))
        return false;

    // Call LoadLibraryW in the remote process to load the DLL
    HANDLE hThread = CreateRemoteThread(m_ProcessHandle, nullptr, 0,
        reinterpret_cast<LPTHREAD_START_ROUTINE>(LoadLibraryW),
        alloc, 0, nullptr);

    //Check if the thread exists
    if (!hThread)
    {
        //Free the memory if it failed
        VirtualFreeEx(m_ProcessHandle, alloc, 0x1000, MEM_RELEASE);
        return false;
    }

    // Wait for the thread to finish executing
    WaitForSingleObject(hThread, INFINITE);

    // Close the thread handle
    CloseHandle(hThread);
	return true;
}

bool Inject::Setup() noexcept
{
    //Get a handle to the process
	m_ProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, false, m_ProcessID);
	if (!m_ProcessHandle)
		return false;

    //Check if empty
	if (m_DllPath.empty())
		return false;

    //You can also add a check, if it is actually a DLL
    //whatever..

	return true;
}
