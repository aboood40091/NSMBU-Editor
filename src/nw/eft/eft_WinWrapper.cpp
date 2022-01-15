#ifdef EFT_WIN

#include <nw/eft/eft_WinWrapper.h>
#include <nw/eft/eft_Heap.h>

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <sstream>

#include <file.hpp>
#include <globals.hpp>
#include <md5.hpp>

static GLuint CompileShader(const char* source, GLenum type)
{
    GLuint id = glCreateShader(type);
    glShaderSource(id, 1, &source, NULL);
    glCompileShader(id);

    GLint result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        GLint length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        char* message = new char[length];
        glGetShaderInfoLog(id, length, &length, message);
        printf("Failed to compile %s shader\n", type == GL_VERTEX_SHADER ? "vertex" : "fragment");
        printf("%s\n", message);
        delete[] message;

        glDeleteShader(id);
        return GL_NONE;
    }

    return id;
}

static GLuint CompileProgram(const char* vertexShader,
                             const char* fragmentShader)
{
    GLuint program = glCreateProgram();
    GLuint vs = CompileShader(vertexShader, GL_VERTEX_SHADER);
    GLuint fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    GLint result;
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    if (result == GL_FALSE)
    {
        GLint length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

        char* message = new char[length];
        glGetProgramInfoLog(program, length, &length, message);
        printf("Failed to link program\n");
        printf("%s\n", message);
        delete[] message;

        glDeleteProgram(program);

        glDeleteShader(vs);
        glDeleteShader(fs);

        return GL_NONE;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

static size_t SaveGX2VertexShader(nw::eft::Heap* heap,
                                  GX2VertexShader* shader,
                                  void** pShaderBuf)
{
    size_t shaderBufSize = sizeof(GX2VertexShader);

    const size_t uniformBlocksOffs = shaderBufSize;
    shaderBufSize += shader->numUniformBlocks * sizeof(GX2UniformBlock);

    const size_t uniformVarsOffs = shaderBufSize;
    shaderBufSize += shader->numUniforms * sizeof(GX2UniformVar);

    const size_t initialValuesOffs = shaderBufSize;
    shaderBufSize += shader->numInitialValues * sizeof(GX2UniformInitialValue);

    const size_t loopVarsOffs = shaderBufSize;
    shaderBufSize += shader->numLoops * sizeof(u32) * 2;

    const size_t samplerVarsOffs = shaderBufSize;
    shaderBufSize += shader->numSamplers * sizeof(GX2SamplerVar);

    const size_t attribVarsOffs = shaderBufSize;
    shaderBufSize += shader->numAttribs * sizeof(GX2AttribVar);

    const size_t strBaseOffs = shaderBufSize;

    for (u32 i = 0; i < shader->numUniformBlocks; i++)
    {
        const char* const name = shader->uniformBlocks[i].name;
        if (name != NULL)
            shaderBufSize += std::strlen(name) + 1;
    }

    for (u32 i = 0; i < shader->numUniforms; i++)
    {
        const char* const name = shader->uniformVars[i].name;
        if (name != NULL)
            shaderBufSize += std::strlen(name) + 1;
    }

    for (u32 i = 0; i < shader->numSamplers; i++)
    {
        const char* const name = shader->samplerVars[i].name;
        if (name != NULL)
            shaderBufSize += std::strlen(name) + 1;
    }

    for (u32 i = 0; i < shader->numAttribs; i++)
    {
        const char* const name = shader->attribVars[i].name;
        if (name != NULL)
            shaderBufSize += std::strlen(name) + 1;
    }

    const size_t shaderDataOffs = shaderBufSize;
    shaderBufSize += shader->shaderSize;

    u8* shaderBuf = (u8*)heap->Alloc(shaderBufSize);

    std::memcpy(shaderBuf, shader, sizeof(GX2VertexShader));
    std::memcpy(shaderBuf + shaderDataOffs, shader->shaderPtr, shader->shaderSize);

    *(u32*)(shaderBuf + offsetof(GX2VertexShader, shaderPtr)) = shaderDataOffs;

    if (shader->numUniformBlocks != 0)
    {
        std::memcpy(shaderBuf + uniformBlocksOffs,
                    shader->uniformBlocks,
                    shader->numUniformBlocks * sizeof(GX2UniformBlock));

        *(u32*)(shaderBuf + offsetof(GX2VertexShader, uniformBlocks)) = uniformBlocksOffs;
    }

    if (shader->numUniforms != 0)
    {
        std::memcpy(shaderBuf + uniformVarsOffs,
                    shader->uniformVars,
                    shader->numUniforms * sizeof(GX2UniformVar));

        *(u32*)(shaderBuf + offsetof(GX2VertexShader, uniformVars)) = uniformVarsOffs;
    }

    if (shader->numInitialValues != 0)
    {
        std::memcpy(shaderBuf + initialValuesOffs,
                    shader->initialValues,
                    shader->numInitialValues * sizeof(GX2UniformInitialValue));

        *(u32*)(shaderBuf + offsetof(GX2VertexShader, initialValues)) = initialValuesOffs;
    }

    if (shader->numLoops != 0)
    {
        std::memcpy(shaderBuf + loopVarsOffs,
                    shader->loopVars,
                    shader->numLoops * sizeof(u32) * 2);

        *(u32*)(shaderBuf + offsetof(GX2VertexShader, loopVars)) = loopVarsOffs;
    }

    if (shader->numSamplers != 0)
    {
        std::memcpy(shaderBuf + samplerVarsOffs,
                    shader->samplerVars,
                    shader->numSamplers * sizeof(GX2SamplerVar));

        *(u32*)(shaderBuf + offsetof(GX2VertexShader, samplerVars)) = samplerVarsOffs;
    }

    if (shader->numAttribs != 0)
    {
        std::memcpy(shaderBuf + attribVarsOffs,
                    shader->attribVars,
                    shader->numAttribs * sizeof(GX2AttribVar));

        *(u32*)(shaderBuf + offsetof(GX2VertexShader, attribVars)) = attribVarsOffs;
    }

    size_t strOffs = strBaseOffs;

    for (u32 i = 0; i < shader->numUniformBlocks; i++)
    {
        const char* const name = shader->uniformBlocks[i].name;
        if (name != NULL)
        {
            const size_t nameLen = std::strlen(name) + 1;
            std::memcpy(shaderBuf + strOffs, name, nameLen);

            *(u32*)(shaderBuf +
                    uniformBlocksOffs
                    + i * sizeof(GX2UniformBlock)
                    + offsetof(GX2UniformBlock, name)) = strOffs;

            strOffs += nameLen;
        }
    }

    for (u32 i = 0; i < shader->numUniforms; i++)
    {
        const char* const name = shader->uniformVars[i].name;
        if (name != NULL)
        {
            const size_t nameLen = std::strlen(name) + 1;
            std::memcpy(shaderBuf + strOffs, name, nameLen);

            *(u32*)(shaderBuf +
                    uniformVarsOffs
                    + i * sizeof(GX2UniformVar)
                    + offsetof(GX2UniformVar, name)) = strOffs;

            strOffs += nameLen;
        }
    }

    for (u32 i = 0; i < shader->numSamplers; i++)
    {
        const char* const name = shader->samplerVars[i].name;
        if (name != NULL)
        {
            const size_t nameLen = std::strlen(name) + 1;
            std::memcpy(shaderBuf + strOffs, name, nameLen);

            *(u32*)(shaderBuf +
                    samplerVarsOffs
                    + i * sizeof(GX2SamplerVar)
                    + offsetof(GX2SamplerVar, name)) = strOffs;

            strOffs += nameLen;
        }
    }

    for (u32 i = 0; i < shader->numAttribs; i++)
    {
        const char* const name = shader->attribVars[i].name;
        if (name != NULL)
        {
            const size_t nameLen = std::strlen(name) + 1;
            std::memcpy(shaderBuf + strOffs, name, nameLen);

            *(u32*)(shaderBuf +
                    attribVarsOffs
                    + i * sizeof(GX2AttribVar)
                    + offsetof(GX2AttribVar, name)) = strOffs;

            strOffs += nameLen;
        }
    }

    *pShaderBuf = shaderBuf;
    return shaderBufSize;
}

static size_t SaveGX2PixelShader(nw::eft::Heap* heap,
                                 GX2PixelShader* shader,
                                 void** pShaderBuf)
{
    size_t shaderBufSize = sizeof(GX2PixelShader);

    const size_t uniformBlocksOffs = shaderBufSize;
    shaderBufSize += shader->numUniformBlocks * sizeof(GX2UniformBlock);

    const size_t uniformVarsOffs = shaderBufSize;
    shaderBufSize += shader->numUniforms * sizeof(GX2UniformVar);

    const size_t initialValuesOffs = shaderBufSize;
    shaderBufSize += shader->numInitialValues * sizeof(GX2UniformInitialValue);

    const size_t loopVarsOffs = shaderBufSize;
    shaderBufSize += shader->numLoops * sizeof(u32) * 2;

    const size_t samplerVarsOffs = shaderBufSize;
    shaderBufSize += shader->numSamplers * sizeof(GX2SamplerVar);

    const size_t strBaseOffs = shaderBufSize;

    for (u32 i = 0; i < shader->numUniformBlocks; i++)
    {
        const char* const name = shader->uniformBlocks[i].name;
        if (name != NULL)
            shaderBufSize += std::strlen(name) + 1;
    }

    for (u32 i = 0; i < shader->numUniforms; i++)
    {
        const char* const name = shader->uniformVars[i].name;
        if (name != NULL)
            shaderBufSize += std::strlen(name) + 1;
    }

    for (u32 i = 0; i < shader->numSamplers; i++)
    {
        const char* const name = shader->samplerVars[i].name;
        if (name != NULL)
            shaderBufSize += std::strlen(name) + 1;
    }

    const size_t shaderDataOffs = shaderBufSize;
    shaderBufSize += shader->shaderSize;

    u8* shaderBuf = (u8*)heap->Alloc(shaderBufSize);

    std::memcpy(shaderBuf, shader, sizeof(GX2PixelShader));
    std::memcpy(shaderBuf + shaderDataOffs, shader->shaderPtr, shader->shaderSize);

    *(u32*)(shaderBuf + offsetof(GX2PixelShader, shaderPtr)) = shaderDataOffs;

    if (shader->numUniformBlocks != 0)
    {
        std::memcpy(shaderBuf + uniformBlocksOffs,
                    shader->uniformBlocks,
                    shader->numUniformBlocks * sizeof(GX2UniformBlock));

        *(u32*)(shaderBuf + offsetof(GX2PixelShader, uniformBlocks)) = uniformBlocksOffs;
    }

    if (shader->numUniforms != 0)
    {
        std::memcpy(shaderBuf + uniformVarsOffs,
                    shader->uniformVars,
                    shader->numUniforms * sizeof(GX2UniformVar));

        *(u32*)(shaderBuf + offsetof(GX2PixelShader, uniformVars)) = uniformVarsOffs;
    }

    if (shader->numInitialValues != 0)
    {
        std::memcpy(shaderBuf + initialValuesOffs,
                    shader->initialValues,
                    shader->numInitialValues * sizeof(GX2UniformInitialValue));

        *(u32*)(shaderBuf + offsetof(GX2PixelShader, initialValues)) = initialValuesOffs;
    }

    if (shader->numLoops != 0)
    {
        std::memcpy(shaderBuf + loopVarsOffs,
                    shader->loopVars,
                    shader->numLoops * sizeof(u32) * 2);

        *(u32*)(shaderBuf + offsetof(GX2PixelShader, loopVars)) = loopVarsOffs;
    }

    if (shader->numSamplers != 0)
    {
        std::memcpy(shaderBuf + samplerVarsOffs,
                    shader->samplerVars,
                    shader->numSamplers * sizeof(GX2SamplerVar));

        *(u32*)(shaderBuf + offsetof(GX2PixelShader, samplerVars)) = samplerVarsOffs;
    }

    size_t strOffs = strBaseOffs;

    for (u32 i = 0; i < shader->numUniformBlocks; i++)
    {
        const char* const name = shader->uniformBlocks[i].name;
        if (name != NULL)
        {
            const size_t nameLen = std::strlen(name) + 1;
            std::memcpy(shaderBuf + strOffs, name, nameLen);

            *(u32*)(shaderBuf +
                    uniformBlocksOffs
                    + i * sizeof(GX2UniformBlock)
                    + offsetof(GX2UniformBlock, name)) = strOffs;

            strOffs += nameLen;
        }
    }

    for (u32 i = 0; i < shader->numUniforms; i++)
    {
        const char* const name = shader->uniformVars[i].name;
        if (name != NULL)
        {
            const size_t nameLen = std::strlen(name) + 1;
            std::memcpy(shaderBuf + strOffs, name, nameLen);

            *(u32*)(shaderBuf +
                    uniformVarsOffs
                    + i * sizeof(GX2UniformVar)
                    + offsetof(GX2UniformVar, name)) = strOffs;

            strOffs += nameLen;
        }
    }

    for (u32 i = 0; i < shader->numSamplers; i++)
    {
        const char* const name = shader->samplerVars[i].name;
        if (name != NULL)
        {
            const size_t nameLen = std::strlen(name) + 1;
            std::memcpy(shaderBuf + strOffs, name, nameLen);

            *(u32*)(shaderBuf +
                    samplerVarsOffs
                    + i * sizeof(GX2SamplerVar)
                    + offsetof(GX2SamplerVar, name)) = strOffs;

            strOffs += nameLen;
        }
    }

    *pShaderBuf = shaderBuf;
    return shaderBufSize;
}

static bool GX2UniformBlockComp(const GX2UniformBlock& a, const GX2UniformBlock& b)
{
    return a.location > b.location;
}

static void ReplaceString(std::string& str, const std::string& a, const std::string& b)
{
    size_t pos = 0;
    while ((pos = str.find(a, pos)) != std::string::npos)
    {
         str.replace(pos, a.length(), b);
         pos += b.length();
    }
}

static void DecompileProgram(nw::eft::Heap* heap,
                             GX2VertexShader* vertexShader,
                             GX2PixelShader* pixelShader,
                             std::string* vertexShaderSrc,
                             std::string* fragmentShaderSrc)
{
    void* vertexShaderBuf;
    size_t vertexShaderBufSize = SaveGX2VertexShader(heap, vertexShader, &vertexShaderBuf);

    void* pixelShaderBuf;
    size_t pixelShaderBufSize = SaveGX2PixelShader(heap, pixelShader, &pixelShaderBuf);

    u8* shaderBuf = (u8*)heap->Alloc(vertexShaderBufSize + pixelShaderBufSize);
    std::memcpy(shaderBuf, vertexShaderBuf, vertexShaderBufSize);
    std::memcpy(shaderBuf + vertexShaderBufSize, pixelShaderBuf, pixelShaderBufSize);

    std::string key = md5(shaderBuf, vertexShaderBufSize + pixelShaderBufSize);

    std::string glVertexShader;
    std::string glFragmentShader;

    ShaderCacheMap::const_iterator it = g_ShaderCache.find(key);
    if (it != g_ShaderCache.end())
    {
        const ShaderCache& shaderCache = it->second;
        glVertexShader = shaderCache.vertexShader;
        glFragmentShader = shaderCache.fragmentShader;
    }
    else
    {
        const std::string basePath = g_CafeCachePath + '/' + key;

        const std::string vertexShaderPath = basePath + "VS";
        const std::string fragmentShaderPath = basePath + "FS";

        const std::string vertexShaderSrcPath = vertexShaderPath + ".vert";
        const std::string fragmentShaderSrcPath = fragmentShaderPath + ".frag";

        if (FileExists(vertexShaderSrcPath.c_str()) && FileExists(fragmentShaderSrcPath.c_str()))
        {
            assert(ReadFile(vertexShaderSrcPath, &glVertexShader));
            assert(ReadFile(fragmentShaderSrcPath, &glFragmentShader));
        }
        else
        {
            const std::string vertexShaderSpirvPath = vertexShaderSrcPath + ".spv";
            const std::string fragmentShaderSpirvPath = fragmentShaderSrcPath + ".spv";

            printf("\n");
            //printf("%s\n", vertexShaderSpirvPath.c_str());
            //printf("%s\n", fragmentShaderSpirvPath.c_str());

            WriteFile(vertexShaderPath.c_str(), (u8*)vertexShaderBuf, vertexShaderBufSize);
            WriteFile(fragmentShaderPath.c_str(), (u8*)pixelShaderBuf, pixelShaderBufSize);

            std::string cmd;

            {
                std::ostringstream cmdStrm;
                cmdStrm << "\"" << g_CafePath << "/gx2shader-decompiler.exe\" -v \"" << vertexShaderPath << "\" -p \"" << fragmentShaderPath << "\"";
                cmd = cmdStrm.str();
            }

            printf("%s\n", cmd.c_str());
            RunCommand(cmd.c_str());

            assert(FileExists(vertexShaderSpirvPath.c_str()));
            assert(FileExists(fragmentShaderSpirvPath.c_str()));

            DeleteFile(vertexShaderPath.c_str());
            DeleteFile(fragmentShaderPath.c_str());

            {
                std::ostringstream cmdStrm;
                cmdStrm << "\"" << g_CafePath << "/spirv-cross.exe\" \"" << vertexShaderSpirvPath << "\" --no-es "  \
                    "--no-420pack-extension --no-support-nonzero-baseinstance " \
                    "--version 410 --output \"" << vertexShaderSrcPath << "\"";
                cmd = cmdStrm.str();
            }

            printf("%s\n", cmd.c_str());
            RunCommand(cmd.c_str());

            assert(FileExists(vertexShaderSrcPath.c_str()));

            {
                std::ostringstream cmdStrm;
                cmdStrm << "\"" << g_CafePath << "/spirv-cross.exe\" \"" << fragmentShaderSpirvPath << "\" --no-es "  \
                    "--no-420pack-extension --no-support-nonzero-baseinstance " \
                    "--version 410 --output \"" << fragmentShaderSrcPath << "\"";
                cmd = cmdStrm.str();
            }

            printf("%s\n", cmd.c_str());
            RunCommand(cmd.c_str());

            assert(FileExists(fragmentShaderSrcPath.c_str()));

            DeleteFile(vertexShaderSpirvPath.c_str());
            DeleteFile(fragmentShaderSpirvPath.c_str());

            assert(ReadFile(vertexShaderSrcPath, &glVertexShader));
            assert(ReadFile(fragmentShaderSrcPath, &glFragmentShader));

            ReplaceString(glVertexShader, "\r\n", "\n");
            ReplaceString(glFragmentShader, "\r\n", "\n");

            std::vector<GX2UniformBlock> vertexUBOs = std::vector<GX2UniformBlock>(vertexShader->uniformBlocks,
                                                                                   vertexShader->uniformBlocks + vertexShader->numUniformBlocks);

            std::sort(vertexUBOs.begin(), vertexUBOs.end(), GX2UniformBlockComp);

            for (u32 i = 0; i < vertexShader->numUniformBlocks; i++)
            {
                std::ostringstream formatOldStrm;
                formatOldStrm << "layout(std430) readonly buffer CBUFFER_DATA_" << vertexUBOs[i].location << std::endl
                              << "{" << std::endl
                              << "    vec4 values[];" << std::endl
                              << "}";

                std::ostringstream formatNewStrm;
                formatNewStrm << "layout(std140) uniform " << vertexUBOs[i].name << std::endl
                              << "{" << std::endl
                              << "    vec4 values[" << ((vertexUBOs[i].size + 15) / 16) << "];" << std::endl
                              << "}";

                ReplaceString(glVertexShader, formatOldStrm.str(), formatNewStrm.str());
            }

            std::vector<GX2UniformBlock> pixelUBOs = std::vector<GX2UniformBlock>(pixelShader->uniformBlocks,
                                                                                  pixelShader->uniformBlocks + pixelShader->numUniformBlocks);

            std::sort(pixelUBOs.begin(), pixelUBOs.end(), GX2UniformBlockComp);

            for (u32 i = 0; i < pixelShader->numUniformBlocks; i++)
            {
                std::ostringstream formatOldStrm;
                formatOldStrm << "layout(std430) readonly buffer CBUFFER_DATA_" << pixelUBOs[i].location << std::endl
                              << "{" << std::endl
                              << "    vec4 values[];" << std::endl
                              << "}";

                std::ostringstream formatNewStrm;
                formatNewStrm << "layout(std140) uniform " << pixelUBOs[i].name << std::endl
                              << "{" << std::endl
                              << "    vec4 values[" << ((pixelUBOs[i].size + 15) / 16) << "];" << std::endl
                              << "}";

                ReplaceString(glFragmentShader, formatOldStrm.str(), formatNewStrm.str());
            }

            WriteFile(vertexShaderSrcPath, glVertexShader);
            WriteFile(fragmentShaderSrcPath, glFragmentShader);
        }

        g_ShaderCache.insert(ShaderCacheMap::value_type(key, ShaderCache(glVertexShader, glFragmentShader)));
    }

    heap->Free(vertexShaderBuf);
    heap->Free(pixelShaderBuf);
    heap->Free(shaderBuf);

    *vertexShaderSrc = glVertexShader;
    *fragmentShaderSrc = glFragmentShader;
}

namespace nw { namespace eft {

TextureSampler::TextureSampler()
{
    glGenSamplers(1, &sampler);

    glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_R, GL_REPEAT);
}

TextureSampler::~TextureSampler()
{
    if (sampler != GL_NONE)
    {
        glDeleteSamplers(1, &sampler);
        sampler = GL_NONE;
    }
}

bool TextureSampler::Setup(TextureFilterMode filterMode, TextureWrapMode wrapModeX, TextureWrapMode wrapModeY)
{
    if (filterMode == TextureFilterMode_Linear)
    {
        glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
    else
    {
        glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    }

    GLenum clampX = GL_MIRRORED_REPEAT;
    switch(wrapModeX)
    {
    case TextureWrapMode_Mirror:
        clampX = GL_MIRRORED_REPEAT;
        break;
    case TextureWrapMode_Wrap:
        clampX = GL_REPEAT;
        break;
    case TextureWrapMode_Clamp:
        clampX = GL_CLAMP_TO_EDGE;
        break;
    case TextureWrapMode_Mirror_Once:
        clampX = GL_MIRROR_CLAMP_TO_EDGE;
        break;
    }

    GLenum clampY = GL_MIRRORED_REPEAT;
    switch(wrapModeY)
    {
    case TextureWrapMode_Mirror:
        clampY = GL_MIRRORED_REPEAT;
        break;
    case TextureWrapMode_Wrap:
        clampY = GL_REPEAT;
        break;
    case TextureWrapMode_Clamp:
        clampY = GL_CLAMP_TO_EDGE;
        break;
    case TextureWrapMode_Mirror_Once:
        clampY = GL_MIRROR_CLAMP_TO_EDGE;
        break;
    }

    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, clampX);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, clampY);
    return true;
}

bool TextureSampler::SetupLOD(f32 maxLOD, f32 biasLOD)
{
    glSamplerParameterf(sampler, GL_TEXTURE_MIN_LOD, 0.0f);
    glSamplerParameterf(sampler, GL_TEXTURE_MAX_LOD, maxLOD);
    glSamplerParameterf(sampler, GL_TEXTURE_LOD_BIAS, biasLOD);
    return true;
}

VertexBuffer::VertexBuffer()
{
    bufferSize = 0;
    buffer = NULL;
}

void* VertexBuffer::AllocateVertexBuffer(Heap* heap, u32 bufSize, u32 elemSize)
{
    glGenBuffers(1, &gl_buffer);

    bufferSize = bufSize;
    buffer = heap->Alloc(bufferSize);
    elementSize = elemSize;
    return buffer;
}

void VertexBuffer::Finalize(Heap* heap)
{
    if (buffer != NULL)
    {
        heap->Free(buffer);
        buffer = NULL;
    }

    if (gl_buffer != GL_NONE)
    {
        glDeleteBuffers(1, &gl_buffer);
        gl_buffer = GL_NONE;
    }
}

void VertexBuffer::Invalidate()
{
    if (buffer == NULL || bufferSize < sizeof(u32))
        return;

    u32* buf_32 = (u32*)buffer;
    for (u32 i = 0; i < bufferSize; i += sizeof(u32))
    {
        *buf_32 = __builtin_bswap32(*buf_32);
        buf_32++;
    }
}

void VertexBuffer::BindBuffer(u32 index, u32, u32 stride)
{
    if (index == 0xFFFFFFFF)
        return;

    // assert(stride == 4 * elementSize);

    glBindBuffer(GL_ARRAY_BUFFER, gl_buffer);
    glBufferData(GL_ARRAY_BUFFER, bufferSize, buffer, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, elementSize, GL_FLOAT, GL_FALSE, stride, (void*)0);
}

void VertexBuffer::BindExtBuffer(u32 index, u32 elemSize, u32 stride, u32 offset)
{
    if (index == 0xFFFFFFFF)
        return;

    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, elemSize, GL_FLOAT, GL_FALSE, stride, (void*)offset);
}

Shader::Shader()
{
    vertexShader = NULL;
    pixelShader = NULL;
    // geometryShader = NULL;
    initialized = false;

    gfd = NULL;
    program = GL_NONE;
}

void Shader::Finalize(Heap* heap)
{
    if (gfd != NULL)
    {
        gfd->~GFDFile();
        heap->Free(gfd);

        vertexShader = NULL;
        pixelShader = NULL;
        // geometryShader = NULL;
    }

    if (program != GL_NONE)
    {
        glDeleteProgram(program);
        program = GL_NONE;

        initialized = false;
    }
}

void Shader::BindShader()
{
    if (program != GL_NONE)
        glUseProgram(program);
}

bool Shader::CreateShader(Heap* heap, const void* binary, u32 binarySize)
{
    gfd = new (heap->Alloc(sizeof(GFDFile))) GFDFile;
    assert(gfd->load(binary) == binarySize);
    //printf("Shader, binary size: %d\n", binarySize);

    vertexShader = &gfd->mVertexShaders[0];
    pixelShader = &gfd->mPixelShaders[0];
    //geometryShader = gfd->mGeometryShaders.size() > 0 ? &gfd->mGeometryShaders[0] : NULL;

    assert(vertexShader != NULL);
    assert(pixelShader != NULL);
    assert(gfd->mGeometryShaders.size() == 0);

    std::string vertexShaderSrc;
    std::string fragmentShaderSrc;
    DecompileProgram(heap,
                     vertexShader,
                     pixelShader,
                     &vertexShaderSrc,
                     &fragmentShaderSrc);

    program = CompileProgram(vertexShaderSrc.c_str(), fragmentShaderSrc.c_str());
    if (program == GL_NONE)
        return false;

    BindShader();

    glUniform4f(glGetUniformLocation(program, "VS_PUSH.posMulAdd"), 1.0f, -1.0f, 0.0f, 0.0f);
    glUniform4f(glGetUniformLocation(program, "VS_PUSH.zSpaceMul"), 0.0f,  1.0f, 1.0f, 1.0f);
    glUniform1f(glGetUniformLocation(program, "VS_PUSH.pointSize"), 1.0f);

    glUniform1ui(glGetUniformLocation(program, "PS_PUSH.alphaFunc"),        7);
    glUniform1f (glGetUniformLocation(program, "PS_PUSH.alphaRef"),         0.5f);
    glUniform1ui(glGetUniformLocation(program, "PS_PUSH.needsPremultiply"), 0);

    initialized = true;

    return true;
}

s32 Shader::GetFragmentSamplerLocation(const char* name)
{
    if (!initialized)
        return 0xFFFFFFFF;

    for (u32 i = 0; i < pixelShader->numSamplers; i++)
    {
        if (std::strcmp(pixelShader->samplerVars[i].name, name) == 0)
        {
            assert(pixelShader->samplerVars[i].type == 1);
            u32 location = pixelShader->samplerVars[i].location;
            std::ostringstream samplerNameStrm;
            samplerNameStrm << "SPIRV_Cross_CombinedTEXTURE_" << location << "SAMPLER_" << location;
            return glGetUniformLocation(program, samplerNameStrm.str().c_str());
        }
    }

    return 0xFFFFFFFF;
}

u32 Shader::GetAttribute(const char* name, u32 buffer, VertexFormat attribFormat, u32 offset, bool instanceID)
{
    if (!initialized)
        return 0xFFFFFFFF;

    for (u32 i = 0; i < vertexShader->numAttribs; i++)
    {
        if (std::strcmp(vertexShader->attribVars[i].name, name) == 0)
        {
            std::ostringstream attribNameStrm;
            attribNameStrm << name << "_0_0";
            return glGetAttribLocation(program, attribNameStrm.str().c_str());
        }
    }

    return 0xFFFFFFFF;
}

bool UniformBlock::InitializeVertexUniformBlock(Shader* shader, const char* name, u32 binding)
{
    if (!shader->initialized)
    {
failure:
        blockNotExist = true;
        return false;
    }

    gl_binding = binding;

    location = glGetUniformBlockIndex(shader->program, name);
    if (location == GL_INVALID_INDEX)
        goto failure;

    for (u32 i = 0; i < shader->vertexShader->numUniformBlocks; i++)
    {
        if (std::strcmp(shader->vertexShader->uniformBlocks[i].name, name) == 0)
        {
            bufferSize = shader->vertexShader->uniformBlocks[i].size;
            goto continue_1;
        }
    }

    goto failure;

continue_1:
    shaderStage = ShaderStage_Vertex;

    glGenBuffers(1, &gl_buffer);
    glBindBuffer(GL_UNIFORM_BUFFER, gl_buffer);
    glBufferData(GL_UNIFORM_BUFFER, bufferSize, NULL, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, gl_binding, gl_buffer);
    glUniformBlockBinding(shader->program, location, gl_binding);

    initialized = true;

    return true;
}

bool UniformBlock::InitializePixelUniformBlock(Shader* shader, const char* name, u32 binding)
{
    if (!shader->initialized)
    {
failure:
        blockNotExist = true;
        return false;
    }

    gl_binding = binding;

    location = glGetUniformBlockIndex(shader->program, name);
    if (location == GL_INVALID_INDEX)
        goto failure;

    for (u32 i = 0; i < shader->pixelShader->numUniformBlocks; i++)
    {
        if (std::strcmp(shader->pixelShader->uniformBlocks[i].name, name) == 0)
        {
            bufferSize = shader->pixelShader->uniformBlocks[i].size;
            goto continue_1;
        }
    }

    goto failure;

continue_1:
    shaderStage = ShaderStage_Fragment;

    glGenBuffers(1, &gl_buffer);
    glBindBuffer(GL_UNIFORM_BUFFER, gl_buffer);
    glBufferData(GL_UNIFORM_BUFFER, bufferSize, NULL, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, gl_binding, gl_buffer);
    glUniformBlockBinding(shader->program, location, gl_binding);

    initialized = true;

    return true;
}

void UniformBlock::BindUniformBlock(const void* buffer)
{
    if (!initialized || bufferSize == 0)
        return;

    glBindBuffer(GL_UNIFORM_BUFFER, gl_buffer);
    glBindBufferBase(GL_UNIFORM_BUFFER, gl_binding, gl_buffer);
    glBufferData(GL_UNIFORM_BUFFER, bufferSize, buffer, GL_DYNAMIC_DRAW);
}

} } // namespace nw::eft

#endif // EFT_WIN
