#pragma once

#include <string>
#include <unordered_map>


extern const std::string g_CWD;
extern const std::string g_CafePath;
extern const std::string g_CafeCachePath;

struct ShaderCache
{
    ShaderCache(const std::string& v, const std::string& f)
        : vertexShader(v)
        , fragmentShader(f)
    {
    }

    ~ShaderCache();

    std::string vertexShader;
    std::string fragmentShader;
};
typedef std::unordered_map<std::string, const ShaderCache> ShaderCacheMap;
extern ShaderCacheMap g_ShaderCache;


void RunCommand(const char* cmd);
