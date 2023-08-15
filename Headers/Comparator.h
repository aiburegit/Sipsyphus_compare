#include <iostream>
#include <vector>
#include "Package.h"
#include <map>
using namespace std;
class Comparator
{
private:
     vector<Package> first_enty;
     vector<Package> second_enty;

     vector<Package> unic_first;
     vector<Package> unic_second;
     
     map<string, vector<Package>> sortByArchVersions(vector<Package> pack){
        map<string, vector<Package>> result;
        for(Package p: pack){
            string arch = p.arch;
            result[arch].push_back(p);
        }
       return result;
     };
public:
    Comparator(Package  _first_enty, Package  _second_enty){
        first_enty.push_back(_first_enty);
        second_enty.push_back(_second_enty);
    };
    Comparator( vector<Package>  _first_enty, vector<Package>  _second_enty){
        first_enty = _first_enty;
        second_enty = _second_enty;
    };

    bool compare(){ 
        auto sort_first_enty = sortByArchVersions(first_enty); 
        auto sort_second_enty = sortByArchVersions(first_enty); 
        for(size_t i = 0;i < first_enty.size();i++){
            for(size_t j = 0;j < second_enty.size();j++){
            
            } 
        }
     return true;
    };
};
