#include "../Headers/Comparator.h"

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
    if (f_item.release == s_item.release)
    {
        int res = rpmvercmp(f_item.version.c_str(), s_item.version.c_str());
        if (res > 0)
        {
            return true;
        }
    }
    return false;
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
                // cout << f_enty.first << endl;
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
                        Comparator::unic_second[f_enty.first].push_back(s_item);
                        Comparator::total_second_unic++;
                    }
                    else
                    {
                        if (Comparator::versionCompare((*iter), s_item))
                        {
                            Comparator::version_diff.push_back((*iter));
                            Comparator::unic_diff[f_enty.first].push_back((*iter));
                            Comparator::total_ver_diff++;
                        }
                        total_count++;
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
                        Comparator::unic_first[f_enty.first].push_back(f_item);
                        Comparator::total_first_unic++;
                    }
                    else
                    {
                        if (Comparator::versionCompare(f_item, (*iter)))
                        {
                            Comparator::version_diff.push_back(f_item);
                            Comparator::unic_diff[f_enty.first].push_back(f_item);
                            Comparator::total_ver_diff++;
                        }
                        total_count++;
                    }
                }
            }
        }
    }
    cout << total_first_unic << endl;
    cout << total_second_unic << endl;
    // cout << total_count<< endl;
    // cout << total_count+total_first_unic+total_second_unic<< endl;
    cout << total_count;
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