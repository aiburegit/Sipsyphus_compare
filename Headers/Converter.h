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
    Document toJSON(Result result);
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
    Value buff_value;
    buff_value.SetString(package.name.data(),package.name.size());
    result.AddMember("name", buff_value, allocator);
    buff_value.SetInt(package.epoch);
    result.AddMember("epoch", buff_value, allocator);
    buff_value.SetString(package.version.data(),package.version.size());
    result.AddMember("version", buff_value, allocator);
    buff_value.SetString(package.release.data(),package.release.size());
    result.AddMember("release", buff_value, allocator);
    buff_value.SetString(package.arch.data(),package.arch.size());
    result.AddMember("arch", buff_value, allocator);
    buff_value.SetString(package.disttag.data(),package.disttag.size());
    result.AddMember("disttag", buff_value, allocator);
    buff_value.SetInt(package.buildtime);
    result.AddMember("buildtime", buff_value, allocator);
    buff_value.SetString(package.source.data(),package.source.size());
    result.AddMember("source", buff_value, allocator);
    return result;
}
Value Converter::getJsonArray(vector<Package> &packages, MemoryPoolAllocator<> &allocator)
{
    Value result;
    result.SetArray();
    for (auto p : packages)
    {
        result.PushBack(getJsonPackage(p,allocator),allocator);
    }
    return result;
}
Value Converter::getUnic(Unic &unic_pack, MemoryPoolAllocator<> &allocator)
{
    Value result;
    Value packages;
    Value buff_value;
    result.SetObject();
    string name = unic_pack.name;
    buff_value.SetString(name.data(),name.size());
    result.AddMember("name",buff_value,allocator);
    buff_value.SetInt(unic_pack.count);
    result.AddMember("count",buff_value,allocator);
    packages.SetArray();
    for (auto p : unic_pack.unic_packages)
    {
        packages.PushBack(getJsonPackage(p,allocator),allocator);
    }
    result.AddMember("packages",packages,allocator);
    return result;
}
Document Converter::toJSON(Result result)
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
        
        buff_value.SetString(arch.name.data(),arch.name.size());
        json_arch.AddMember("name",buff_value,allocator);
        buff_value.SetInt(arch.first_count);
        string name_first = arch.first_unic_pac.name + "_count";
        name_buff_value.SetString(name_first.data(),name_first.size());
        json_arch.AddMember(name_buff_value,buff_value,allocator);

        string name_second = arch.second_unic_pac.name + "_count";
        buff_value.SetInt(arch.second_count);
        name_buff_value.SetString(name_second.data(),name_second.size());
        json_arch.AddMember(name_buff_value,buff_value,allocator);

        buff_value.SetInt(arch.first_dominate_count);
        json_arch.AddMember("first_dominate",buff_value,allocator);

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
    cout << strbuf.GetString();

    return doc;
}
