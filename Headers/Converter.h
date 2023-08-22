#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "Package.h"
#include <vector>
#include <iostream>
using namespace rapidjson;
class Converter
{
private:
    /* data */
public:
    Converter();
    ~Converter();
    Package parseToPackage(const Value &_package);
    vector<Package> getPackages(const string api_result);
};

Converter::Converter(/* args */)
{
}

Converter::~Converter()
{
}
Package Converter::parseToPackage(const Value &_package)
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

