#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "Package.h"
#include "Result.h"
#include <vector>
#include <iostream>
using namespace rapidjson;
class Converter
{
private:
    Value getJsonArray(vector<Package> &packages, MemoryPoolAllocator<> &allocator);
    Value getJsonPackage(Package &package, MemoryPoolAllocator<> &allocator);
    Value getUnic(Unic &unic_pack, MemoryPoolAllocator<> &allocator);

public:
    Converter();
    ~Converter();
    Package parseToPackage(const Value &_package);
    vector<Package> getPackages(const string api_result);
    string toJSON(Result result);
};

Converter::Converter(/* args */)
{
}

Converter::~Converter()
{
}
Package Converter::parseToPackage(const Value &_package)
{
    Package pack{
        _package["name"].GetString(),
        _package["epoch"].GetInt(),
        _package["version"].GetString(),
        _package["release"].GetString(),
        _package["arch"].GetString(),
        _package["disttag"].GetString(),
        _package["buildtime"].GetInt(),
        _package["source"].GetString()};
    return pack;
}
vector<Package> Converter::getPackages(const string api_result)
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
Value Converter::getJsonPackage(Package &package, MemoryPoolAllocator<> &allocator)
{
    Value result;
    result.SetObject();
    Value name(package.name.c_str(),allocator);
    result.AddMember("name", name, allocator);
    Value epoc(package.epoch);
    result.AddMember("epoch", epoc, allocator);
    Value version(package.version.c_str(),allocator);
    result.AddMember("version", version, allocator);
    Value release(package.release.c_str(),allocator);
    result.AddMember("release", release, allocator);
    Value arch(package.arch.c_str(),allocator);
    result.AddMember("arch", arch, allocator);
    Value disttag(package.disttag.c_str(),allocator);
    result.AddMember("disttag", disttag, allocator);
    Value buildtime(package.buildtime);
    result.AddMember("buildtime", buildtime, allocator);
    Value source(package.source.c_str(),allocator);
    result.AddMember("source", source, allocator);
    return result;
}
Value Converter::getJsonArray(vector<Package> &packages, MemoryPoolAllocator<> &allocator)
{
    Value result;
    result.SetArray();
    for (auto &p : packages)
    {
        result.PushBack(getJsonPackage(p,allocator),allocator);
    }
    return result;
}
Value Converter::getUnic(Unic &unic_pack, MemoryPoolAllocator<> &allocator)
{
    Value result;
    Value packages;
    result.SetObject();
    Value name(unic_pack.name.c_str(),allocator);
    result.AddMember("name",name,allocator);
    Value unic_count(unic_pack.count);
    result.AddMember("count",unic_count,allocator);
    packages.SetArray();
    for (auto &p : unic_pack.unic_packages)
    {
        packages.PushBack(getJsonPackage(p,allocator),allocator);
    }
    result.AddMember("packages",packages,allocator);
    return result;
}
string Converter::toJSON(Result result)
{
    Document doc;
    Value json_val;
    auto &allocator = doc.GetAllocator();
    doc.SetObject();
    json_val.SetInt(result.total_first_unic);
    doc.AddMember("total_sisyphus_unic", json_val, allocator);
    json_val.SetInt(result.total_second_unic);
    doc.AddMember("total_p10_unic", json_val, allocator);
    json_val.SetInt(result.total_first_dominate);
    doc.AddMember("sisyphus_ver_dominate", json_val, allocator);
    json_val.SetArray();
    for (auto arch : result.arches)
    {
        Value buff_value;
        Value name_buff_value;
        Value json_arch;
        json_arch.SetObject();
        Value json_unic_first;
        Value json_unic_second;
        Value json_unic_dominate;
        
        Value arch_name(arch.name.c_str(),allocator);
        json_arch.AddMember("name",arch_name,allocator);

        Value name_first(arch.first_unic_pac.name.c_str(),allocator);
        Value first_count(arch.first_count);
        json_arch.AddMember(name_first,first_count,allocator);

        
       
        Value name_second (arch.second_unic_pac.name.c_str(),allocator);
        Value second_count(arch.second_count);
        json_arch.AddMember(name_second,second_count,allocator);

        Value first_dom_count(arch.first_dominate_count);
        json_arch.AddMember("first_dominate",first_dom_count,allocator);

        json_unic_first = getUnic(arch.first_unic_pac,allocator);
        json_arch.AddMember("first_packages", json_unic_first, allocator);

        json_unic_second = getUnic(arch.second_unic_pac,allocator);
        json_arch.AddMember("second_packages", json_unic_second, allocator);

        json_unic_dominate = getUnic(arch.dominate_unic_pac,allocator);
        json_arch.AddMember("first_dominate_packages", json_unic_dominate, allocator);

        json_val.PushBack(json_arch, allocator);
    }
    doc.AddMember("arches",json_val,allocator);
    StringBuffer strbuf;
    Writer<StringBuffer> writer(strbuf);
    doc.Accept(writer);
    return strbuf.GetString();
}
