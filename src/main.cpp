#include <iostream>
#include <string>
#include <vector>
#include <atomic>
#include <cstdlib>
#include <windows.h>
#include "ConfigLoader.h"
#include "EnvironmentLauncher.h"
#include "NetworkSinkHole.h"
#include "ProcessManager.h"

// Atomic flag ensures cleanup runs exactly once, no matter which exit path fires.
static std::atomic<bool> hasRestored{false};
volatile bool isFocusing = true;

// Central cleanup: restores the hosts file exactly once.
static void PerformCleanup() {
    // compare_exchange_strong: if hasRestored is false, set it to true and proceed.
    // If another thread/handler already set it, this becomes a no-op.
    bool expected = false;
    if (hasRestored.compare_exchange_strong(expected, true)) {
        RestoreWebsite();
        std::cout << "\n*** Focus Session Ended ***" << std::endl;
    }
}

// Last-resort safety net registered via atexit().
static void AtExitCleanup() {
    PerformCleanup();
}

// Crash handler: catches unhandled SEH exceptions (e.g. access violations).
static LONG WINAPI CrashHandler(EXCEPTION_POINTERS*) {
    PerformCleanup();
    return EXCEPTION_CONTINUE_SEARCH;  // Let Windows handle the crash after cleanup.
}

BOOL WINAPI ConsoleHandler(DWORD signal) {
    if (signal == CTRL_C_EVENT || signal == CTRL_CLOSE_EVENT ||
        signal == CTRL_LOGOFF_EVENT || signal == CTRL_SHUTDOWN_EVENT) {
        std::cout << "\n Exit Signal Caught! Cleaning up..." << std::endl;
        isFocusing = false;

        // Always perform cleanup in the handler itself. For CLOSE/LOGOFF/
        // SHUTDOWN events Windows may kill the process almost immediately
        // after this handler returns, so we cannot rely on main() continuing.
        // For CTRL_C_EVENT, the main loop *may* still exit cleanly — the
        // atomic flag ensures we only restore once either way.
        PerformCleanup();
        return TRUE;
    }
    return FALSE;
}

int main() {

    // Register multiple safety nets so cleanup runs no matter how we exit.
    SetConsoleCtrlHandler(ConsoleHandler, TRUE);
    SetUnhandledExceptionFilter(CrashHandler);
    std::atexit(AtExitCleanup);

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

    //4. Restore (no-op if cleanup already ran in a signal handler)
    PerformCleanup();
    return 0;
}