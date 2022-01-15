#pragma once

#include <cstdlib>
#include <string>
#include <tr1/unordered_map>

#include <nw/eft/eft_Handle.h>
#include <nw/eft/eft_Heap.h>


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
typedef std::tr1::unordered_map<std::string, const ShaderCache> ShaderCacheMap;
extern ShaderCacheMap g_ShaderCache;


extern int g_WindowWidth;
extern int g_WindowHeight;


class EftHeap : public nw::eft::Heap
{
public:
    EftHeap() { }
    virtual ~EftHeap() { }

    virtual void* Alloc(u32 size, s32 alignment = 4)
    {
        return calloc(size, 1);
    }

    virtual void Free(void* ptr)
    {
        free(ptr);
    }
};

extern EftHeap g_EftRootHeap;

namespace nw { namespace eft {

class Handle;
class System;

} }

extern nw::eft::System* g_EftSystem;
extern nw::eft::Handle g_EftHandle;


void RunCommand(const char* cmd);
