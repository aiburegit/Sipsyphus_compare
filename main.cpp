#define CURL_STATICLIB
#include <iostream>
#include <curl/curl.h>
#include "algorithm"
#include "Headers/Httprequest.h"
#include "Headers/Package.h"
#include "Headers/Comparator.h"
#include "Headers/Converter.h"
#include "Headers/Result.h"
#include <vector>
using namespace std;

int main(int argc, char *argv[])
{
    string def_url = "https://rdb.altlinux.org/api/export/branch_binary_packages/";
    string first_request;
    string second_request;
    string name_first;
    string name_second;
    if(argc < 3){
        cout << "Enter a API URLs\n";
        return -1;
    }
    if(argc == 4){
        if(strcmp(argv[1],"-u") == 0){
            first_request =  argv[2];
            second_request = argv[3];
            name_first = first_request.substr(first_request.find_last_of('/')+1);
            name_second = second_request.substr(second_request.find_last_of('/')+1);
        }
    }
    if(argc == 3){
        first_request = def_url + argv[1];
        second_request = def_url + argv[2];
        name_first = argv[1];
        name_second = argv[2];
    }
   
    Result result;
    Converter converter;
    vector<Package> first_pack;
    vector<Package> second_pack;
    // cout << "Geting " << name_first << " packages.." << endl;
    const auto first_result = Httprequest::httpGet(first_request.c_str());
    first_pack = converter.getPackages(first_result);
    if(first_pack.size() == 0){
        cout << "First url error\n";
        cout << first_result +'\n';
        return -1;
    }
    // cout << "Total sisyphus: " << first_pack.size() << endl;
    const auto second_result = Httprequest::httpGet(second_request.c_str());
    second_pack = converter.getPackages(second_result);
    if(second_pack.size() == 0){
        cout << "Second url error\n";
        cout << second_result + '\n';
        return -1;
    }
    // cout << "Geting " << name_second <<  "packages.." << endl;
    
    // cout << "Total p10: " << second_pack.size() << endl;
    

    Comparator comp(first_pack, second_pack);
    comp.compare();
    result.total_first_unic = comp.getTotalFirst();
    result.total_second_unic = comp.getTotalSecond();
    result.total_first_dominate = comp.getTotalDiff();
    auto first_dominate_packs = comp.getDiffUnic();
    for(auto f_item : comp.getFirstUnic()){
        for(auto s_item : comp.getSecondUnic()){
            if(f_item.first == s_item.first){
                Arch arch;
                arch.name = f_item.first;
                arch.first_count = f_item.second.size();
                arch.second_count = s_item.second.size();
                arch.first_dominate_count = first_dominate_packs[f_item.first].size();

                Unic unic_first;
                unic_first.name = name_first;
                unic_first.count = f_item.second.size();
                unic_first.unic_packages = f_item.second;

                Unic unic_second;
                unic_second.name = name_second;
                unic_second.count = s_item.second.size();
                unic_second.unic_packages = s_item.second;

                Unic unic_dominate;
                unic_dominate.name = "dominate_"+name_first;
                unic_dominate.count = first_dominate_packs[f_item.first].size();
                unic_dominate.unic_packages = first_dominate_packs[f_item.first];

                arch.first_unic_pac = unic_first;
                arch.second_unic_pac = unic_second;
                arch.dominate_unic_pac = unic_dominate;
                result.arches.push_back(arch);
            }
        }
    }
    auto js = converter.toJSON(result);
    cout << js;
    return 0;
}