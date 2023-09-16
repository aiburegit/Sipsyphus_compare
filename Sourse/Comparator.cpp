#include "../Headers/Comparator.h"
#include <fstream>

Comparator::Comparator(Package _first_enty, Package _second_enty)
{
    first_enty.push_back(_first_enty);
    second_enty.push_back(_second_enty);
};
Comparator::Comparator(vector<Package> _first_enty, vector<Package> _second_enty)
{
    first_enty = _first_enty;
    second_enty = _second_enty;
};

map<string, vector<Package>> Comparator::sortByArchVersions(vector<Package> pack)
{
    map<string, vector<Package>> result;
    for (Package p : pack)
    {
        string arch = p.arch;
        result[arch].push_back(p);
    }
    return result;
};
bool Comparator::versionCompare(Package f_item, Package s_item)
{
    //lead to version-release
    string f_ver_rel = f_item.version+"-"+f_item.release;
    string s_ver_rel = s_item.version+"-"+s_item.release;
    int res_ver = rpmvercmp(f_ver_rel.c_str(), s_ver_rel.c_str());
    if (res_ver > 0)
    {
        return true;
    }
    return false;
}
int Comparator::getTotalSum(map<string, vector<Package>> unic_container)
{
    int result = 0;
    for (auto p : unic_container)
    {
        result += p.second.size();
    }
    return result;
}
void Comparator::packagesGetDiff(pair<const string, vector<Package>> f_enty, pair<const string, vector<Package>> s_enty, map<string, vector<Package>> *diff_container)
{
    unordered_set<Package, PackageHash> f_pac_set;
    for (auto f_item : f_enty.second)
    {
        f_pac_set.insert(f_item);
    }
    for (auto s_item : s_enty.second)
    {
        auto iter = f_pac_set.find(s_item);
        if (iter != f_pac_set.end())
        {
            if (versionCompare((*iter), s_item))
            {
                (*diff_container)[f_enty.first].push_back((*iter));
            }
        }
    }
}
void Comparator::packagesGetUnic(pair<const string, vector<Package>> f_enty, pair<const string, vector<Package>> s_enty, map<string, vector<Package>> *unic_container)
{
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
            (*unic_container)[f_enty.first].push_back(f_item);
        }
    }
}
bool Comparator::compare()
{
    Comparator::total_first_unic = 0;
    Comparator::total_second_unic = 0;
    Comparator::total_ver_diff = 0;
    int total_count = 0;

    auto sort_first_enty = Comparator::sortByArchVersions(Comparator::first_enty);
    auto sort_second_enty = Comparator::sortByArchVersions(Comparator::second_enty);
    for (auto f_enty : sort_first_enty)
    {
        for (auto s_enty : sort_second_enty)
        {
            if (f_enty.first == s_enty.first)
            {
                packagesGetUnic(f_enty, s_enty, &unic_first);
                packagesGetUnic(s_enty, f_enty, &unic_second);
                packagesGetDiff(f_enty, s_enty, &unic_diff);
            }
        }
    }
    total_first_unic = getTotalSum(unic_first);
    total_second_unic = getTotalSum(unic_second);
    total_ver_diff = getTotalSum(unic_diff);
    return true;
}
map<string, vector<Package>> Comparator::getFirstUnic()
{
    return this->unic_first;
}

map<string, vector<Package>> Comparator::getSecondUnic()
{
    return this->unic_second;
}
map<string, vector<Package>> Comparator::getDiffUnic()
{
    return this->unic_diff;
}
int Comparator::getTotalFirst()
{
    return this->total_first_unic;
}
int Comparator::getTotalSecond()
{
    return this->total_second_unic;
}
int Comparator::getTotalDiff()
{
    return this->total_ver_diff;
}