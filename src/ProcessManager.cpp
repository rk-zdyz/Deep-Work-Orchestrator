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

    int killCount = 0;

    if(Process32FirstW(hSnapShot, &entry)){
        do{
            if(_wcsicmp(entry.szExeFile, processName) == 0){
                HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, entry.th32ProcessID);
                if (hProcess != NULL){
                    TerminateProcess(hProcess, 0);
                    CloseHandle(hProcess);
                    killCount++;
                } else{
                    std::wcerr << L"[!] Failed to open process: " << processName << std::endl;
                }
            } 
        } while (Process32NextW(hSnapShot, &entry));
    }
    CloseHandle(hSnapShot);

    if(killCount > 0){
        std::wcout << L"Killed " << killCount << L" instance(s) of " << processName << std::endl;
    }
}