#include <globals.hpp>

#include <algorithm>

#include <windows.h>


static inline std::string GetCWD()
{
    std::string path = std::string(MAX_PATH, '\0');
    GetModuleFileNameA(NULL, &path[0], path.length());
    std::replace(path.begin(), path.end(), '\\', '/');
    return path.substr(0, path.find_last_of('/'));
}

const std::string g_CWD = GetCWD();
const std::string g_CafePath = g_CWD + "/Cafe";
const std::string g_CafeCachePath = g_CafePath + "/Cache";


int g_WindowWidth = 1;
int g_WindowHeight = 1;


EftHeap g_EftRootHeap;
nw::eft::System* g_EftSystem = NULL;
nw::eft::Handle g_EftHandle;


ShaderCache::~ShaderCache()
{
}

ShaderCacheMap g_ShaderCache;


void RunCommand(const char* cmd)
{
    STARTUPINFOA si = { sizeof(STARTUPINFOA), 0 };
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;

    PROCESS_INFORMATION pi = { 0 };

    if (CreateProcessA(NULL, const_cast<char*>(cmd), NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
    {
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
    }
}
