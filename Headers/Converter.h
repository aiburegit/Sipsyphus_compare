#pragma once
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
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
    Package parseToPackage(const Value &_package);
    vector<Package> getPackages(const string api_result);
    string toJSON(Result result);
};