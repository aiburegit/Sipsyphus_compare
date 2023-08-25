#pragma once
#include <iostream>
#include "curl/curl.h"
using namespace std;
 class Httprequest
{
private:
  static  size_t WriteCallback(void* contents, size_t size, size_t nmemb,string* response);
public:
  static const string httpGet(const char* url);
};

