#include <file.hpp>

#include <fstream>
#include <new>

bool ReadFile(const char* filename, u8** out_data, u32* out_size)
{
    if (!out_data && !out_size)
        return false;

    std::ifstream inf(filename, std::ifstream::in | std::ifstream::binary);

    inf.seekg(0, std::ios::end);
    const u32 inf_size = inf.tellg();
    inf.seekg(0, std::ios::beg);

    if (inf_size == 0xFFFFFFFF)
    {
        inf.close();
        return false;
    }

    if (out_data)
    {
        u8* const inb = new u8[inf_size];
        if (!inb)
            return false;

        inf.read((char*)inb, inf_size);

        *out_data = inb;
    }

    if (out_size)
        *out_size = inf_size;

    inf.close();
    return true;
}

bool WriteFile(const char* filename, u8* data, u32 size)
{
    if (!data || !size)
        return false;

    std::ofstream outf(filename, std::ofstream::out | std::ofstream::binary);

    outf.write((char*)data, size);
    outf.close();

    return true;
}

void FreeFile(const void* data)
{
    delete[] (u8*)data;
}

bool ReadFile(const std::string& filename, std::string* out_str)
{
    if (!out_str)
        return false;

    std::ifstream inf(filename.c_str(), std::ifstream::in | std::ifstream::binary);

    inf.seekg(0, std::ios::end);
    const u32 inf_size = inf.tellg();
    inf.seekg(0, std::ios::beg);

    if (inf_size == 0xFFFFFFFF)
    {
        inf.close();
        return false;
    }

    u8* const inb = new u8[inf_size];
    if (!inb)
        return false;

    inf.read((char*)inb, inf_size);
    inf.close();

    *out_str = std::string((const char*)inb, inf_size);

    delete[] inb;

    return true;
}

bool WriteFile(const std::string& filename, const std::string& str)
{
    std::ofstream outf(filename.c_str(), std::ofstream::out);
    outf << str;
    outf.close();

    return true;
}

bool FileExists(const char* path)
{
    DWORD dwAttrib = GetFileAttributes(path);
    return dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
}
