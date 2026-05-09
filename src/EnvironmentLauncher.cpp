#include "EnvironmentLauncher.h"
#include <windows.h>
#include <iostream>

void LaunchWorkSpace(const std::vector<std::string>& appsToLaunch){
    std::cout << "Deploying development environment.. " << std::endl;
    for(const std::string& app : appsToLaunch) {
        std::cout << "Launching " << app << std::endl;
        HINSTANCE result = ShellExecuteA(NULL, "open", app.c_str(), NULL, NULL, SW_SHOWNORMAL);
        if((INT_PTR)result <=32){
            std::cout << "[!] Failed to launch " << app << ". Ensure it is installed and in your PATH." << std::endl;
        }
    }
    std::cout << "All applications launched successfully." << std::endl;
}