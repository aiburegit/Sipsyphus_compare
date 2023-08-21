#include <iostream>
#include <vector>
#include "Package.h"
#include <map>
#include <unordered_set>
#include <algorithm>
#include <stdlib.h>
#include <rpm/rpmlib.h>
#include <rpm/rpmvercmp.h>
#include <rpm/header.h>
#include <rpm/rpmlegacy.h>

using namespace std;
class Comparator
{
private:
    vector<Package> first_enty;
    vector<Package> second_enty;
    map<string,vector<Package>> unic_first;
    map<string,vector<Package>> unic_second;
    vector<Package> version_diff;
    int total_first_unic = 0, total_second_unic = 0, total_ver_diff = 0;
    map<string, vector<Package>> sortByArchVersions(vector<Package> pack)
    {
        map<string, vector<Package>> result;
        for (Package p : pack)
        {
            string arch = p.arch;
            result[arch].push_back(p);
        }
        return result;
    };
    bool versionCompare(Package f_item, Package s_item)  //returns True if the version of the first package is greater than the second
    {
        if (f_item.release == s_item.release)
        {
            int res = rpmvercmp(f_item.version.c_str(), s_item.version.c_str());
            if(res > 0){
                return true;
            }
        }
        return false;
    }

public:
    Comparator(Package _first_enty, Package _second_enty)
    {
        first_enty.push_back(_first_enty);
        second_enty.push_back(_second_enty);
    };
    Comparator(vector<Package> _first_enty, vector<Package> _second_enty)
    {
        first_enty = _first_enty;
        second_enty = _second_enty;
    };

    bool compare()
    {
        total_first_unic = 0;
        total_second_unic = 0;
        total_ver_diff = 0;

        auto sort_first_enty = sortByArchVersions(first_enty);
        auto sort_second_enty = sortByArchVersions(second_enty);
        for (auto f_enty : sort_first_enty)
        {
            for (auto s_enty : sort_second_enty)
            {
                if (f_enty.first == s_enty.first)
                {
                    cout << f_enty.first << endl;
                    unordered_set<Package, PackageHash> f_pac_set;
                    for (auto f_item : f_enty.second)
                    {
                        f_pac_set.insert(f_item);
                    }
                    for (auto s_item : s_enty.second)
                    {
                        auto iter = f_pac_set.find(s_item);
                        if (iter == f_pac_set.end())
                        {
                            unic_second[f_enty.first].push_back(s_item);
                            total_second_unic++;
                        }
                        else
                        {
                            if(versionCompare((*iter),s_item)){
                                version_diff.push_back((*iter));
                                total_ver_diff++;
                            }
                        }
                    }
                    unordered_set<Package, PackageHash> s_pac_set;
                    for (auto s_item : s_enty.second)
                    {
                        s_pac_set.insert(s_item);
                    }
                    for (auto f_item : f_enty.second)
                    {
                        auto iter = s_pac_set.find(f_item);
                        if (iter == s_pac_set.end())
                        {
                            unic_first[f_enty.first].push_back(f_item);
                            total_first_unic++;
                        }
                        else
                        {
                            if(versionCompare(f_item,(*iter))){
                                version_diff.push_back(f_item);
                                total_ver_diff++;
                            }
                        }
                    }
                }
            }
        }
        return true;
    };

    map<string, vector<Package>>getFirstUnic(){
        return this->unic_first;
    }

    map<string, vector<Package>>getSecondUnic(){
        return this->unic_second;
    }
    int getTotalFirst(){
        return this->total_first_unic;
    }
    int getTotalSecond(){
        return this->total_second_unic;
    }
    int getTotalDiff(){
        return this->total_ver_diff;
    }

};
