#include "NetworkSinkHole.h"
#include <fstream>
#include <iostream>
#include <string>
#include <windows.h>

const std::string HOSTS_PATH = "C:\\Windows\\System32\\drivers\\etc\\hosts";
const std::string BACKUP_PATH = "C:\\Windows\\System32\\drivers\\etc\\hosts.backup";

void BlockWebsite(const std::vector<std::string>& domainsToBlock) {
    std::cout << "Sinkholing Distracting Website.. " << std::endl;
    CopyFileA(HOSTS_PATH.c_str(), BACKUP_PATH.c_str(), FALSE);

    std::ofstream hostsFile(HOSTS_PATH, std::ios::app);
    if(hostsFile.is_open()){
        hostsFile << "\n# Deep-Work Orchestrator \n";
        for(const std::string& domain : domainsToBlock) {
            hostsFile << "127.0.0.1 " << domain << "\n";
            hostsFile << "127.0.0.1 www." << domain << "\n";
        }
        hostsFile.close();
        std::cout << "Websites blocked successfully" << std::endl;
    } else {
        std::cout << "Error. Could not open hosts file. Are you running as Administrator?" << std::endl;
    }
}

void RestoreWebsite(){
    std::cout << "Restoring network rules.." << std::endl;
    CopyFileA(BACKUP_PATH.c_str(), HOSTS_PATH.c_str(), FALSE);
    DeleteFileA(BACKUP_PATH.c_str());
    // Flush DNS cache so the restored hosts file takes effect immediately
    system("ipconfig /flushdns > nul 2>&1");
}