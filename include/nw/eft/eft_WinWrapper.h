#ifndef EFT_WIN_WRAPPER_H_
#define EFT_WIN_WRAPPER_H_

#ifdef EFT_WIN

#include "eft_Enum.h"

#include <GL/glew.h>
#include <cafe/gfd.h>
#include <cafe.h>

namespace nw { namespace eft {

class Drawer
{
public:
    enum
    {
        TRIANGLES      = GL_TRIANGLES,
        TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
        QUADS          = GL_QUADS,
    };

    static void DrawArraysInstanced(u32 mode, u32 count, u32 instanceCount, u32 first = 0)
    {
        glDrawArraysInstanced(mode, first, count, instanceCount);
    }

    static void DrawElementsInstanced(u32 mode, u32 count, const void* indices, u32 instanceCount)
    {
        glDrawElementsInstanced(mode, count, GL_UNSIGNED_INT, indices, instanceCount);
    }

    static void DrawArrays(u32 mode, u32 count, u32 first = 0)
    {
        DrawArraysInstanced(mode, count, 1, first);
    }

    static void DrawElements(u32 mode, u32 count, const void* indices)
    {
        DrawElementsInstanced(mode, count, indices, 1);
    }
};

class Heap;

class Shader
{
public:
    enum VertexFormat
    {
        VertexFormat_U32,
        VertexFormat_VEC3,
        VertexFormat_VEC4,
    };

public:
    Shader();

    void Finalize(Heap* heap);
    void BindShader();
    bool CreateShader(Heap* heap, const void* binary, u32 binarySize);
    s32 GetFragmentSamplerLocation(const char* name);
    s32 GetAttributeLocation(const char* name) { return -1; }
    u32 GetAttribute(const char* name, u32 buffer, VertexFormat attribFormat, u32 offset, bool instanceID);
    void SetupShader(Heap* heap) {}

    GX2VertexShader* vertexShader;
    GX2PixelShader* pixelShader;
    //GX2GeometryShader* geometryShader;
    bool initialized;

    GFDFile* gfd;
    GLuint program;
};

class TextureSampler
{
public:
    TextureSampler();
    ~TextureSampler();

    bool Setup(TextureFilterMode filterMode, TextureWrapMode wrapModeX, TextureWrapMode wrapModeY);
    bool SetupLOD(f32 maxLOD, f32 biasLOD);

    GLuint sampler;
};

class UniformBlock
{
public:
    enum ShaderStage
    {
        ShaderStage_Vertex   = 0,
        ShaderStage_Fragment = 1,
        ShaderStage_Geometry = 2,
        ShaderStage_Max      = 3
    };

public:
    UniformBlock()
    {
        initialized = false;
        blockNotExist = false;
        bufferSize = 0;
        location = 0;
        shaderStage = ShaderStage_Max;

        gl_binding = -1;
        gl_buffer = GL_NONE;
    }

    bool InitializeVertexUniformBlock(Shader* shader, const char* name, u32 binding);
    bool InitializePixelUniformBlock(Shader* shader, const char* name, u32 binding);
    void BindUniformBlock(const void* buffer);

    bool initialized;
    bool blockNotExist;
    u32 bufferSize;
    u32 location;
    ShaderStage shaderStage;

    GLint gl_binding;
    GLuint gl_buffer;
};

class VertexBuffer
{
public:
    VertexBuffer();

    void* AllocateVertexBuffer(Heap* heap, u32 bufSize, u32 elemSize);
    void Finalize(Heap* heap);
    void Invalidate();
    void BindBuffer(u32 index, u32, u32 stride);

    static void BindExtBuffer(u32 index, u32 elemSize, u32 stride, u32 offset);

    u32 elementSize;
    u32 bufferSize;
    void* buffer;

    GLuint gl_buffer;
};

} } // namespace nw::eft

#endif // EFT_WIN

#endif // EFT_WIN_WRAPPER_H_
