#include "ProcessManager.h"
#include <windows.h>
#include <tlhelp32.h>
#include <iostream>

void KillProcessByName(const wchar_t* processName){
    HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapShot == INVALID_HANDLE_VALUE){
        return;
    }
    PROCESSENTRY32W entry;
    entry.dwSize = sizeof(PROCESSENTRY32W);

    if(Process32FirstW(hSnapShot, &entry)){
        do{
            if(wcscmp(entry.szExeFile, processName) == 0){
                std::wcout << L"Found " << processName << L". Termination in process.." << std::endl;
                HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, entry.th32ProcessID);
                if (hProcess != NULL){
                    TerminateProcess(hProcess, 0);
                    std::wcout << L"Process Terminated Successfully" << std::endl;
                    CloseHandle(hProcess);
                } else{
                    std::wcerr << L"Failed to open process" << std::endl;
                }
            } 
        } while (Process32NextW(hSnapShot, &entry));
    }
    CloseHandle(hSnapShot);
}