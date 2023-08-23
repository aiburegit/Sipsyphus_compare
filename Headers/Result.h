#pragma once
#include "Package.h"
#include <vector>
#include <iostream>
struct Unic
{
    string name;
    int count;
    vector<Package> unic_packages;
};
struct Arch
{
    string name;
    int first_count;
    int second_count;
    int first_dominate_count;
    Unic first_unic_pac;
    Unic second_unic_pac;
    Unic dominate_unic_pac;
};
struct Result
{
    int total_first_unic;
    int total_second_unic;
    int total_first_dominate;
    vector<Arch> arches;
};


