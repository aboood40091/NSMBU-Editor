#include <nw/eft/eft_RenderContext.h>
#include <nw/eft/eft_Shader.h>

namespace nw { namespace eft {

RenderContext::RenderContext()
{
#ifdef EFT_WIN
    for (u32 i = 0; i < TextureSlot_Max; i++)
        textureSamplers[i].Setup(TextureFilterMode_Linear, TextureWrapMode_Wrap, TextureWrapMode_Wrap);
    for (u32 i = 0; i < TextureSlot_Max; i++)
        nativeTextureSamplers[i].Setup(TextureFilterMode_Linear, TextureWrapMode_Mirror, TextureWrapMode_Mirror);
#else
    textureSampler.Setup(TextureFilterMode_Linear, TextureWrapMode_Wrap, TextureWrapMode_Wrap);
    nativeTextureSampler.Setup(TextureFilterMode_Linear, TextureWrapMode_Mirror, TextureWrapMode_Mirror);
#endif // EFT_WIN

#ifdef EFT_WIN
    glGenVertexArrays(1, &gl_VAO);
#endif // EFT_WIN
}

#ifdef EFT_WIN

RenderContext::~RenderContext()
{
    if (gl_VAO != GL_NONE)
    {
        glDeleteVertexArrays(1, &gl_VAO);
        gl_VAO = GL_NONE;
    }
}

#endif // EFT_WIN

void RenderContext::SetupCommonState()
{
#ifdef EFT_WIN

    glEnable(GL_TEXTURE_2D);

    glFrontFace(GL_CCW);
    glDisable(GL_CULL_FACE);

    glEnable(GL_BLEND);

    glBindVertexArray(gl_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_NONE);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(4);
    glDisableVertexAttribArray(5);
    glDisableVertexAttribArray(6);
    glDisableVertexAttribArray(7);
    glDisableVertexAttribArray(8);
    glDisableVertexAttribArray(9);
    glDisableVertexAttribArray(10);
    glDisableVertexAttribArray(11);
    glDisableVertexAttribArray(12);
    glDisableVertexAttribArray(13);
    glDisableVertexAttribArray(14);
    glDisableVertexAttribArray(15);

#else

    GX2SetCullOnlyControl(GX2_FRONT_FACE_CCW, GX2_DISABLE, GX2_DISABLE);
    GX2SetColorControl(GX2_LOGIC_OP_COPY, GX2_ENABLE, GX2_DISABLE, GX2_ENABLE);

#endif // EFT_WIN

    blendType = BlendType_Max;
    zBufATestType = ZBufATestType_Max;
}

void RenderContext::SetupBlendType(BlendType blendType)
{
#ifdef EFT_WIN

    switch (blendType)
    {
    case BlendType_Normal:
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBlendEquation(GL_FUNC_ADD);
        break;
    case BlendType_Add:
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glBlendEquation(GL_FUNC_ADD);
        break;
    case BlendType_Sub:
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
        break;
    case BlendType_Screen:
        glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE);
        glBlendEquation(GL_FUNC_ADD);
        break;
    case BlendType_Mult:
        glBlendFunc(GL_ZERO, GL_SRC_COLOR);
        glBlendEquation(GL_FUNC_ADD);
        break;
    }

#else

    switch (blendType)
    {
    case BlendType_Normal:
        GX2SetBlendControl(GX2_RENDER_TARGET_0,
                           GX2_BLEND_SRC_ALPHA,
                           GX2_BLEND_ONE_MINUS_SRC_ALPHA,
                           GX2_BLEND_COMBINE_ADD,
                           GX2_DISABLE,
                           GX2_BLEND_SRC_ALPHA,
                           GX2_BLEND_ONE_MINUS_SRC_ALPHA,
                           GX2_BLEND_COMBINE_ADD);
        break;
    case BlendType_Add:
        GX2SetBlendControl(GX2_RENDER_TARGET_0,
                           GX2_BLEND_SRC_ALPHA,
                           GX2_BLEND_ONE,
                           GX2_BLEND_COMBINE_ADD,
                           GX2_DISABLE,
                           GX2_BLEND_SRC_ALPHA,
                           GX2_BLEND_ONE,
                           GX2_BLEND_COMBINE_ADD);
        break;
    case BlendType_Sub:
        GX2SetBlendControl(GX2_RENDER_TARGET_0,
                           GX2_BLEND_SRC_ALPHA,
                           GX2_BLEND_ONE,
                           GX2_BLEND_COMBINE_DST_MINUS_SRC,
                           GX2_DISABLE,
                           GX2_BLEND_SRC_ALPHA,
                           GX2_BLEND_ONE,
                           GX2_BLEND_COMBINE_DST_MINUS_SRC);
        break;
    case BlendType_Screen:
        GX2SetBlendControl(GX2_RENDER_TARGET_0,
                           GX2_BLEND_ONE_MINUS_DST_COLOR,
                           GX2_BLEND_ONE,
                           GX2_BLEND_COMBINE_ADD,
                           GX2_DISABLE,
                           GX2_BLEND_ONE_MINUS_DST_COLOR,
                           GX2_BLEND_ONE,
                           GX2_BLEND_COMBINE_ADD);
        break;
    case BlendType_Mult:
        GX2SetBlendControl(GX2_RENDER_TARGET_0,
                           GX2_BLEND_ZERO,
                           GX2_BLEND_SRC_COLOR ,
                           GX2_BLEND_COMBINE_ADD,
                           GX2_DISABLE,
                           GX2_BLEND_ONE_MINUS_DST_COLOR,
                           GX2_BLEND_ONE,
                           GX2_BLEND_COMBINE_ADD);
        break;
    }

#endif // EFT_WIN

    this->blendType = blendType;
}

void RenderContext::SetupZBufATest(ZBufATestType zBufATestType)
{
#ifdef EFT_WIN

    switch (zBufATestType)
    {
    case ZBufATestType_Normal:
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.0f);
        glEnable(GL_BLEND);
        break;
    case ZBufATestType_Ignore_Z:
        glDisable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.0f);
        glEnable(GL_BLEND);
        break;
    case ZBufATestType_Alpha:
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.5f);
        glDisable(GL_BLEND);
        break;
    }

#else

    switch (zBufATestType)
    {
    case ZBufATestType_Normal:
        GX2SetDepthOnlyControl(GX2_TRUE, GX2_FALSE, GX2_COMPARE_LEQUAL);
        GX2SetAlphaTest(GX2_TRUE, GX2_COMPARE_GREATER, 0.0f);
        GX2SetColorControl(GX2_LOGIC_OP_COPY, GX2_ENABLE, GX2_DISABLE, GX2_ENABLE);
        break;
    case ZBufATestType_Ignore_Z:
        GX2SetDepthOnlyControl(GX2_FALSE, GX2_FALSE, GX2_COMPARE_LEQUAL);
        GX2SetAlphaTest(GX2_TRUE, GX2_COMPARE_GREATER, 0.0f);
        GX2SetColorControl(GX2_LOGIC_OP_COPY, GX2_ENABLE, GX2_DISABLE, GX2_ENABLE);
        break;
    case ZBufATestType_Alpha:
        GX2SetDepthOnlyControl(GX2_TRUE, GX2_TRUE, GX2_COMPARE_LEQUAL);
        GX2SetAlphaTest(GX2_TRUE, GX2_COMPARE_GREATER, 0.5f);
        GX2SetColorControl(GX2_LOGIC_OP_COPY, GX2_DISABLE, GX2_DISABLE, GX2_ENABLE);
        break;
    }

#endif // EFT_WIN

    this->zBufATestType = zBufATestType;
}

void RenderContext::SetupDisplaySideType(DisplaySideType displaySideType) const
{
#ifdef EFT_WIN

    switch (displaySideType)
    {
    case DisplaySideType_Both:
        glDisable(GL_CULL_FACE);
        break;
    case DisplaySideType_Front:
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        break;
    case DisplaySideType_Back:
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        break;
    }

#else

    switch (displaySideType)
    {
    case DisplaySideType_Both:
        GX2SetCullOnlyControl(GX2_FRONT_FACE_CCW, GX2_FALSE, GX2_FALSE);
        break;
    case DisplaySideType_Front:
        GX2SetCullOnlyControl(GX2_FRONT_FACE_CCW, GX2_FALSE, GX2_TRUE);
        break;
    case DisplaySideType_Back:
        GX2SetCullOnlyControl(GX2_FRONT_FACE_CCW, GX2_TRUE, GX2_FALSE);
        break;
    }

#endif // EFT_WIN
}

#ifdef EFT_WIN

void RenderContext::SetupTexture(const TextureRes* texture, TextureSlot slot, FragmentTextureLocation samplerLocation)
{
    if (texture == NULL || texture->gl_texture == GL_NONE || samplerLocation.location == 0xFFFFFFFF)
        return;

    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, texture->gl_texture);

    TextureSampler& textureSampler = textureSamplers[slot];
    textureSampler.Setup(static_cast<TextureFilterMode>(texture->filterMode),
                         static_cast<TextureWrapMode>(texture->wrapMode & 0xF),
                         static_cast<TextureWrapMode>(texture->wrapMode >> 4));
    textureSampler.SetupLOD(texture->maxLOD, texture->biasLOD);

    glBindSampler(slot, textureSampler.sampler);
    glUniform1i(samplerLocation.location, slot);
}

void RenderContext::SetupTexture(GLuint texture, TextureSlot slot, FragmentTextureLocation samplerLocation)
{
    // TODO
}

#else

void RenderContext::_SetupFragmentTexture(const GX2Texture* texture, const GX2Sampler* sampler, TextureSlot slot, FragmentTextureLocation samplerLocation) const
{
    GX2SetPixelTexture(texture, samplerLocation.location);
    GX2SetPixelSampler(sampler, samplerLocation.location);
}

void RenderContext::SetupTexture(const TextureRes* texture, TextureSlot slot, FragmentTextureLocation samplerLocation)
{
    if (texture == NULL || samplerLocation.location == 0xFFFFFFFF)
        return;

    GX2SetPixelTexture(&texture->gx2Texture, samplerLocation.location);

    textureSampler.Setup(static_cast<TextureFilterMode>(texture->filterMode),
                         static_cast<TextureWrapMode>(texture->wrapMode & 0xF),
                         static_cast<TextureWrapMode>(texture->wrapMode >> 4));
    textureSampler.SetupLOD(texture->maxLOD, texture->biasLOD);

    GX2SetPixelSampler(&textureSampler.sampler, samplerLocation.location);
}

void RenderContext::SetupTexture(const GX2Texture* texture, TextureSlot slot, FragmentTextureLocation samplerLocation)
{
    _SetupFragmentTexture(texture, &nativeTextureSampler.sampler, slot, samplerLocation);
}

#endif // EFT_WIN

} } // namespace nw::eft
