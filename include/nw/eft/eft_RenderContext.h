#ifndef EFT_RENDER_CONTEXT_H_
#define EFT_RENDER_CONTEXT_H_

#ifdef EFT_WIN
#include "eft_WinWrapper.h"
#else
#include "eft_CafeWrapper.h"
#endif // EFT_WIN

#include "eft_Enum.h"

namespace nw { namespace eft {

#ifdef EFT_WIN

#define EFT_NATIVE_TEXTURE_NONE GL_NONE
typedef GLuint NativeTexture;
typedef GLuint NativeSampler;

#else

#define EFT_NATIVE_TEXTURE_NONE ((const GX2Texture*)NULL)
typedef const GX2Texture* NativeTexture;
typedef const GX2Sampler* NativeSampler;

#endif // EFT_WIN

struct TextureRes;
struct FragmentTextureLocation;

class RenderContext
{
public:
    RenderContext();

    void SetupCommonState();
    void SetupBlendType(BlendType blendType);
    void SetupZBufATest(ZBufATestType zBufATestType);
    void SetupDisplaySideType(DisplaySideType displaySideType) const;
    inline void _SetupFragmentTexture(NativeTexture texture, NativeSampler sampler, TextureSlot slot, FragmentTextureLocation samplerLocation) const;
    void SetupTexture(const TextureRes* texture, TextureSlot slot, FragmentTextureLocation samplerLocation);
    void SetupTexture(NativeTexture texture, TextureSlot slot, FragmentTextureLocation samplerLocation);

#ifdef EFT_WIN
    ~RenderContext();

    TextureSampler textureSamplers[TextureSlot_Max];
    TextureSampler nativeTextureSamplers[TextureSlot_Max];
#else
    TextureSampler textureSampler;
    TextureSampler nativeTextureSampler;
#endif // EFT_WIN

    BlendType blendType;
    ZBufATestType zBufATestType;

#ifdef EFT_WIN
    GLuint gl_VAO;
#endif // EFT_WIN
};
#ifndef EFT_WIN
static_assert(sizeof(RenderContext) == 0x20, "RenderContext size mismatch");
#endif // EFT_WIN

} } // namespace nw::eft

#endif // EFT_RENDER_CONTEXT_H_
