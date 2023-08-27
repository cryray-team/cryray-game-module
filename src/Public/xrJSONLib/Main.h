#pragma once

#ifdef XRJSON_EXPORTS
#define JSON_API __declspec(dllexport)
#else
#define JSON_API __declspec(dllimport)
#endif

class JSON_API Json
{
public:
    static void CheckJSONDLL();
    static std::string GetStringCheck();
};