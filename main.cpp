#define CURL_STATICLIB
#include <iostream>
#include <curl/curl.h>
#include "rapidjson/document.h"
#include "Headers/Httprequest.h"
#include "Headers/Package.h"
#include "Headers/Comparator.h"
#include <vector>
using namespace rapidjson;
using namespace std;
 Package parseToPackage(const Value& _package){
     std::string name = _package["name"].GetString();
         int epoch = _package["epoch"].GetInt();
         std::string version = _package["version"].GetString();
         std::string release = _package["release"].GetString();
         std::string arch = _package["arch"].GetString();
         std::string disttag = _package["disttag"].GetString();
         int buildtime = _package["buildtime"].GetInt();
         std::string source = _package["source"].GetString();
         Package pack(name, epoch, version, release, arch , disttag, buildtime, source);
         return pack;
 }

 int main() {
    vector<Package> sisyphus;
    vector<Package> p10;
    const auto sisyphus_result = Httprequest::httpGet("https://rdb.altlinux.org/api/export/branch_binary_packages/sisyphus");
    Document doc;
    doc.Parse(sisyphus_result.c_str());
    Value& packages = doc["packages"];
    for(SizeType i = 0;i<packages.Size();i++){
        const Value& package = packages[i];
        Package pack = parseToPackage(package);
        sisyphus.push_back(pack);
    }
    const auto p10_result = Httprequest::httpGet("https://rdb.altlinux.org/api/export/branch_binary_packages/p10");
    doc.Parse(p10_result.c_str());
    packages = doc["packages"];
    for(SizeType i = 0;i<packages.Size();i++){
        const Value& package = packages[i];
        Package pack = parseToPackage(package);
        p10.push_back(pack);
    }
    Comparator comp(sisyphus, p10);
    comp.compare();

    return 0;
}