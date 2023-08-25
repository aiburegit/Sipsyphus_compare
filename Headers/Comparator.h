#pragma once
#include <iostream>
#include <vector>
#include "Package.h"
#pragma once
#include <map>
#include <unordered_set>
#include <algorithm>
#include <stdlib.h>
#include <rpm/rpmlib.h>
#include <rpm/rpmvercmp.h>

using namespace std;
class Comparator
{
private:
    vector<Package> first_enty;
    vector<Package> second_enty;
    map<string,vector<Package>> unic_first;
    map<string,vector<Package>> unic_second;
    map<string,vector<Package>> unic_diff;
    vector<Package> version_diff;
    int total_first_unic = 0, total_second_unic = 0, total_ver_diff = 0;
    map<string, vector<Package>> sortByArchVersions(vector<Package> pack);
    bool versionCompare(Package f_item, Package s_item);  //returns True if the version of the first package is greater than the second
    bool test_method(Package pac);

public:
    Comparator(Package _first_enty, Package _second_enty);
    Comparator(vector<Package> _first_enty, vector<Package> _second_enty);
    bool compare();

    map<string, vector<Package>>getFirstUnic();

    map<string, vector<Package>>getSecondUnic();
    map<string, vector<Package>>getDiffUnic();
    int getTotalFirst();
    int getTotalSecond();
    int getTotalDiff();
};
