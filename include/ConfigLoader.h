#pragma once
#include <vector>
#include <string>

struct Config {
    std::vector<std::string> websites;
    std::vector<std::string> distractions;
    std::vector<std::string> applications;
};
Config LoadConfig(const std::string& path);