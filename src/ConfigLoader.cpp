#include "ConfigLoader.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

static std::string Trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, last - first + 1);
}

Config LoadConfig(const std::string& path) {
    Config config;
    std::ifstream file(path);

    if (!file.is_open()) {
        std::cerr << "[!] Could not open config file: " << path << std::endl;
        return config;
    }

    std::cout << "Loading configuration from " << path << "..." << std::endl;

    enum class Section { NONE, WEBSITES, DISTRACTIONS, APPLICATIONS };
    Section currentSection = Section::NONE;
    std::string line;

    while (std::getline(file, line)) {
        line = Trim(line);

        if (line.empty() || line[0] == '#') {
            continue;
        }

        if (line[0] == '[' && line.back() == ']') {
            std::string sectionName = line.substr(1, line.size() - 2);
            
            std::transform(sectionName.begin(), sectionName.end(),
                           sectionName.begin(), ::tolower);

            if (sectionName == "websites") {
                currentSection = Section::WEBSITES;
            } else if (sectionName == "distractions") {
                currentSection = Section::DISTRACTIONS;
            } else if (sectionName == "applications") {
                currentSection = Section::APPLICATIONS;
            } else {
                std::cerr << "[!] Unknown config section: [" << sectionName << "]" << std::endl;
                currentSection = Section::NONE;
            }
            continue;
        }

        switch (currentSection) {
            case Section::WEBSITES:
                config.websites.push_back(line);
                break;
            case Section::DISTRACTIONS:
                config.distractions.push_back(line);
                break;
            case Section::APPLICATIONS:
                config.applications.push_back(line);
                break;
            case Section::NONE:
                std::cerr << "[!] Entry outside of a section, skipping: " << line << std::endl;
                break;
        }
    }

    file.close();

    std::cout << "  Websites to block:  " << config.websites.size() << std::endl;
    std::cout << "  Apps to launch:     " << config.applications.size() << std::endl;
    std::cout << "  Processes to kill:  " << config.distractions.size() << std::endl;

    return config;
}
