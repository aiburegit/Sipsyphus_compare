#pragma once
#include <iostream>
using namespace std;
struct Package
{
    string name;
    int epoch;
    string version;
    string release;
    string arch;
    string disttag;
    int buildtime;
    string source;
    bool operator==(const Package& pac)const{
        return this->name == pac.name;
    }
    bool operator!=(const Package& pac)const{
        return !(*this == pac);
    }
};
struct PackageHash {
    std::size_t operator()(const Package& pac) const {
        std::size_t h1 = std::hash<string>()(pac.name);
        return h1 ^ (h1 << 1);
    }
};
