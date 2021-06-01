#pragma once

#include <string>
#include <cstdlib>

bool BindEvc(std::string Name, std::string Describe);

std::string MakeDescribes();

inline std::string getEnv(std::string Name) {
    return std::getenv(Name.c_str());
}