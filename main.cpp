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
Document toJSON(int total_sisyph, int total_p10, int total_diff, map<string,vector<Package>>sisyph_arches, map<string,vector<Package>>p10_arches){
    cout << total_diff;
    Document doc;
    Value json_val;
    auto &allocator = doc.GetAllocator();
    doc.SetObject();
    json_val.SetInt(total_sisyph);
    doc.AddMember("total_sisyphus_unic",json_val,allocator);
    json_val.SetInt(total_p10);
    doc.AddMember("total_p10_unic",json_val,allocator);
    json_val.SetInt(total_diff);
    doc.AddMember("sisyphus_ver_dominate",json_val,allocator);
    json_val.SetObject();
    for(auto sis_arch:sisyph_arches){
        for(auto p10_arch:p10_arches){
            if(sis_arch.first == p10_arch.first){
                Value sisyphus_packages;
                Value p10_packages;
                sisyphus_packages.SetObject();
                Value packages;
                packages.SetArray();
                for(auto sis_pack : sis_arch.second){
                    Value package;
                    package.SetObject();
                    Value json_val;
                    json_val.SetString(sis_pack.name.data(),sis_pack.name.size(),allocator);
                    package.AddMember("name", json_val, allocator);
                    json_val.SetString(sis_pack.version.data(),sis_pack.version.size(),allocator);
                    package.AddMember("version",  json_val, allocator);
                    json_val.SetString(sis_pack.arch.data(),sis_pack.arch.size(),allocator);
                    package.AddMember("arch",  json_val, allocator);
                    packages.PushBack(package, allocator);
                }
                Value total;
                total.SetInt(sis_arch.second.size());
                sisyphus_packages.AddMember("name", "sisyphus_unic",allocator);
                sisyphus_packages.AddMember("total_unic", total,allocator);
                sisyphus_packages.AddMember("packages",packages,allocator);
                Value arch;
                arch.SetString(sis_arch.first.data(),sis_arch.first.size(),allocator);
                json_val.AddMember(arch,sisyphus_packages,allocator);
            }
        }
    }
    doc.AddMember("arches", json_val, allocator);
    StringBuffer strbuf;
    Writer<StringBuffer> writer(strbuf);
    doc.Accept(writer);
    return doc;
}


int main()
{
    string first_request = "https://rdb.altlinux.org/api/export/branch_binary_packages/sisyphus";
    auto name_first = first_request.substr(first_request.find_last_of('/')+1);
    string second_request = "https://rdb.altlinux.org/api/export/branch_binary_packages/p10";
    auto name_second = second_request.substr(second_request.find_last_of('/')+1);
    Result result;
    Converter converter;
    vector<Package> first_pack;
    vector<Package> second_pack;
    cout << "Geting " << name_first << " packages.." << endl;
    const auto first_result = Httprequest::httpGet("https://rdb.altlinux.org/api/export/branch_binary_packages/sisyphus");
    first_pack = converter.getPackages(first_result);
    cout << "Total sisyphus: " << first_pack.size() << endl;
    const auto second_result = Httprequest::httpGet("https://rdb.altlinux.org/api/export/branch_binary_packages/p10");
    cout << "Geting " << name_second <<  "packages.." << endl;
    second_pack = converter.getPackages(second_result);
    cout << "Total p10: " << second_pack.size() << endl;
    Comparator comp(first_pack, second_pack);
    comp.compare();
    result.total_first_unic = comp.getTotalFirst();
    result.total_second_unic = comp.getTotalSecond();
    result.total_first_dominate = comp.getTotalDiff();
    for(auto f_item : comp.getFirstUnic()){
        for(auto s_item : comp.getSecondUnic()){
            if(f_item.first == s_item.first){
                Arch arch;
                arch.name = f_item.first;
                arch.first_count = f_item.second.size();
                arch.second_count = s_item.second.size();

                Unic unic_first;
                unic_first.name = name_first;
                unic_first.unic_packages = f_item.second;

                Unic unic_second;
                unic_second.name = name_second;
                unic_second.unic_packages = s_item.second;

                arch.first_unic_pac = unic_first;
                arch.second_unic_pac = unic_second;
                result.arches.push_back(arch);
            }
        }
    }
    return 0;
}