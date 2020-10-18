#include "env.h"
#include <map>

std::map<std::string, std::string> nameToDescibe;

bool BindEvc(std::string Name, std::string Describe) {
    if (nameToDescibe.find(Name) != nameToDescibe.end()) {
        return false;
    }
    nameToDescibe[Name] = Describe;
    return true;
}

std::string MakeDescribes() {
    std::string ret;
    for (auto v:nameToDescibe) {
        ret += v.first;
        ret += "\t" + v.second + "\n";
    }
    return ret;
}