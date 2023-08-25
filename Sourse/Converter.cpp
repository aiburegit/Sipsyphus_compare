#include "../Headers/Converter.h"

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
    try
    {
        vector<Package> result;
        Document doc;
        doc.Parse(api_result.c_str());
        if(doc.FindMember("packages") == doc.MemberEnd()){
            return vector<Package>();
        }
        Value &packages = doc["packages"];
        for (SizeType i = 0; i < packages.Size(); i++)
        {
            const Value &package = packages[i];
            Package pack = parseToPackage(package);
            result.push_back(pack);
        }
        return result;
    }
    catch (...)
    {
         return vector<Package>();
    }
}
Value Converter::getJsonPackage(Package &package, MemoryPoolAllocator<> &allocator)
{
    Value result;
    result.SetObject();
    Value name(package.name.c_str(), allocator);
    result.AddMember("name", name, allocator);
    Value epoc(package.epoch);
    result.AddMember("epoch", epoc, allocator);
    Value version(package.version.c_str(), allocator);
    result.AddMember("version", version, allocator);
    Value release(package.release.c_str(), allocator);
    result.AddMember("release", release, allocator);
    Value arch(package.arch.c_str(), allocator);
    result.AddMember("arch", arch, allocator);
    Value disttag(package.disttag.c_str(), allocator);
    result.AddMember("disttag", disttag, allocator);
    Value buildtime(package.buildtime);
    result.AddMember("buildtime", buildtime, allocator);
    Value source(package.source.c_str(), allocator);
    result.AddMember("source", source, allocator);
    return result;
}
Value Converter::getJsonArray(vector<Package> &packages, MemoryPoolAllocator<> &allocator)
{
    Value result;
    result.SetArray();
    for (auto &p : packages)
    {
        result.PushBack(getJsonPackage(p, allocator), allocator);
    }
    return result;
}
Value Converter::getUnic(Unic &unic_pack, MemoryPoolAllocator<> &allocator)
{
    Value result;
    Value packages;
    result.SetObject();
    Value name(unic_pack.name.c_str(), allocator);
    result.AddMember("name", name, allocator);
    Value unic_count(unic_pack.count);
    result.AddMember("count", unic_count, allocator);
    packages.SetArray();
    for (auto &p : unic_pack.unic_packages)
    {
        packages.PushBack(getJsonPackage(p, allocator), allocator);
    }
    result.AddMember("packages", packages, allocator);
    return result;
}
string Converter::toJSON(Result result)
{
    Document doc;
    Value json_val;
    auto &allocator = doc.GetAllocator();
    doc.SetObject();
    json_val.SetInt(result.total_first_unic);
    Value first_name(result.arches[0].first_unic_pac.name.c_str(), allocator);
    doc.AddMember(first_name, json_val, allocator);
    json_val.SetInt(result.total_second_unic);
    Value second_name(result.arches[0].second_unic_pac.name.c_str(), allocator);
    doc.AddMember(second_name, json_val, allocator);
    json_val.SetInt(result.total_first_dominate);
    Value dominate_name(result.arches[0].dominate_unic_pac.name.c_str(), allocator);
    doc.AddMember(dominate_name, json_val, allocator);
    json_val.SetArray();
    for (auto &arch : result.arches)
    {
        string first_rep_name = arch.first_unic_pac.name;
        string second_rep_name = arch.second_unic_pac.name;
        Value buff_value;
        Value name_buff_value;
        Value json_arch;
        json_arch.SetObject();
        Value json_unic_first;
        Value json_unic_second;
        Value json_unic_dominate;

        Value arch_name(arch.name.c_str(), allocator);
        json_arch.AddMember("name", arch_name, allocator);

        Value name_first(first_rep_name.c_str(), allocator);
        Value first_count(arch.first_count);
        json_arch.AddMember(name_first, first_count, allocator);

        Value name_second(second_rep_name.c_str(), allocator);
        Value second_count(arch.second_count);
        json_arch.AddMember(name_second, second_count, allocator);

        Value first_dom_count(arch.first_dominate_count);
        Value name_dom(string(first_rep_name + "_dominate").c_str(), allocator);
        json_arch.AddMember(name_dom, first_dom_count, allocator);

        json_unic_first = getUnic(arch.first_unic_pac, allocator);
        Value unic_first_name(string(first_rep_name + "_packages").c_str(), allocator);
        json_arch.AddMember(unic_first_name, json_unic_first, allocator);

        json_unic_second = getUnic(arch.second_unic_pac, allocator);
        Value unic_second_name(string(second_rep_name + "_packages").c_str(), allocator);
        json_arch.AddMember(unic_second_name, json_unic_second, allocator);

        json_unic_dominate = getUnic(arch.dominate_unic_pac, allocator);
        Value unic_dom_name(string(first_rep_name + "_dom_packages").c_str(), allocator);
        json_arch.AddMember(unic_dom_name, json_unic_dominate, allocator);

        json_val.PushBack(json_arch, allocator);
    }
    doc.AddMember("arches", json_val, allocator);
    StringBuffer strbuf;
    Writer<StringBuffer> writer(strbuf);
    doc.Accept(writer);
    return strbuf.GetString();
}
