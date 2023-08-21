#define CURL_STATICLIB
#include <iostream>
#include <curl/curl.h>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "Headers/Httprequest.h"
#include "Headers/Package.h"
#include "Headers/Comparator.h"
#include <vector>

using namespace rapidjson;
using namespace std;
Package parseToPackage(const Value &_package)
{
    Package pack
    {
        _package["name"].GetString(),
        _package["epoch"].GetInt(),
        _package["version"].GetString(),
        _package["release"].GetString(),
        _package["arch"].GetString(),
        _package["disttag"].GetString(),
        _package["buildtime"].GetInt(),
        _package["source"].GetString()
    };
    return pack;
}
vector<Package> getPackages(const string api_result)
{
    vector<Package> result;
    Document doc;
    doc.Parse(api_result.c_str());
    Value &packages = doc["packages"];
    for (SizeType i = 0; i < packages.Size(); i++)
    {
        const Value &package = packages[i];
        Package pack = parseToPackage(package);
        result.push_back(pack);
    }
    return result;
}
Document toJSON(int total_sisyph, int total_p10, int total_diff, map<string,vector<Package>>sisyph_arches, map<string,vector<Package>>p10_arches){
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
    vector<Package> sisyphus;
    vector<Package> p10;
    printf("Geting sisyphus packages..\n");
    const auto sisyphus_result = Httprequest::httpGet("https://rdb.altlinux.org/api/export/branch_binary_packages/sisyphus");
    sisyphus = getPackages(sisyphus_result);
    printf("Total sisyphus: %u\n", sisyphus.size());
    const auto p10_result = Httprequest::httpGet("https://rdb.altlinux.org/api/export/branch_binary_packages/p10");
    printf("Geting p10 packages..\n");
    p10 = getPackages(p10_result);
    printf("Total p10: %u\n", p10.size());
    Comparator comp(sisyphus, p10);
    comp.compare();
    toJSON(comp.getTotalFirst(),comp.getTotalSecond(),comp.getTotalDiff(),comp.getFirstUnic(),comp.getSecondUnic());

    return 0;
}