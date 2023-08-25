#include "../Headers/Httprequest.h"

size_t Httprequest::WriteCallback(void *contents, size_t size, size_t nmemb,string *response)
{
    response->append((char *)contents, size * nmemb);
    return size * nmemb;
}
const string Httprequest::httpGet(const char *url)
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
    CURL *curl = curl_easy_init();
    if (curl)
    {
        std::string response;
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            return stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res);
        }
        curl_easy_cleanup(curl);
        return response;
    }
    curl_global_cleanup();
    return "Error";
};