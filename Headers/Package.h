#pragma once

#include <iostream>
using namespace std;
class Package
{
public:
    string name;
    int epoch;
    string version;
    string release;
    string arch;
    string disttag;
    int buildtime;
    string source;

    Package(string _name, int _epoch, string _version, string _release, string _arch, string _disttag, int _buildtime, string _source)
    {
        name = _name;
        epoch = _epoch;
        version = _version;
        release = _release;
        arch = _arch;
        disttag = _disttag;
        buildtime = _buildtime;
        source = _source;
    };
    bool operator==(const Package& f_pack){
        if(this->name == f_pack.name && this->epoch == f_pack.epoch &&
           this->version == f_pack.version && this->release == f_pack.release &&
           this->arch == f_pack.arch && this->disttag == f_pack.disttag &&
           this->buildtime == f_pack.buildtime && this->source == f_pack.source){

            return true;
           }
        return false;
    }
};
