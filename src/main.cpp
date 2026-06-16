#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include "ConfigLoader.h"
#include "EnvironmentLauncher.h"
#include "NetworkSinkHole.h"
#include "ProcessManager.h"

volatile bool isFocusing = true;
BOOL WINAPI ConsoleHandler(DWORD signal) {
    if(signal == CTRL_C_EVENT || signal == CTRL_CLOSE_EVENT ||
       signal == CTRL_LOGOFF_EVENT || signal == CTRL_SHUTDOWN_EVENT){
        std::cout << "\n Exit Signal Caught! Cleaning up..." << std::endl;
        isFocusing = false;

        // For CTRL_CLOSE_EVENT and shutdown events, Windows may kill the
        // process almost immediately after this handler returns. Perform
        // cleanup right here to guarantee the hosts file is restored.
        if(signal != CTRL_C_EVENT){
            RestoreWebsite();
            std::cout << "\n*** Focus Session Ended ***" << std::endl;
        }
        return TRUE;
    }
    return FALSE;
}

int main() {

    SetConsoleCtrlHandler(ConsoleHandler, TRUE);
    std::cout << "*** Welcome to Deep Work Orchestrator ***" << std::endl;
    std::cout << "This tool will:" << std::endl;
    std::cout << "1. Block Distracting Websites" << std::endl;
    std::cout << "2. Launch your focused environment" << std::endl;
    std::cout << "3. Monitor and stop time-wasting apps" << std::endl;
    std::cout << "\nProtecting your focus...\n" << std::endl;

    Config config = LoadConfig("config.ini");

    //1. Sinkholing
    if(!config.websites.empty()) BlockWebsite(config.websites);

    //2. Launch Applications
    if(!config.applications.empty()) LaunchWorkSpace(config.applications);

    //3. Monitor and Kill
    std::cout << "\n*** Focus Mode Activated ***" << std::endl;
    std::cout << "Monitoring for distractions. Press Ctrl + C to exit the session " << std::endl;

    try {
        while(isFocusing){
            for(const std::string& app : config.distractions){

                // Convert std::string to std::wstring for KillProcessByName
                std::wstring wapp(app.begin(), app.end());
                KillProcessByName(wapp.c_str());
            }
            Sleep(5000);
            std::cout << ".";
        }
    } catch (const std::exception& e){
        std::cerr << "Error: " << e.what() << std::endl;
    }

    //4. Restore 
    RestoreWebsite();
    std::cout << "\n*** Focus Session Ended ***" << std::endl;
    return 0;
}