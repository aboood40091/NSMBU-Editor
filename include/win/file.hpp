#pragma once

#include <types.h>

#include <string>

bool ReadFile(const char* filename, u8** out_data, u32* out_size);
bool WriteFile(const char* filename, u8* data, u32 size);
void FreeFile(const void* data);

bool ReadFile(const std::string& filename, std::string* out_str);
bool WriteFile(const std::string& filename, const std::string& str);

bool FileExists(const char* path);

#include <windows.h>
