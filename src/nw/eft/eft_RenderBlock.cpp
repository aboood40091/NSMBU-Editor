#include <nw/eft/eft_Renderer.h>
#include <nw/eft/eft_Shader.h>

namespace nw { namespace eft {

void Renderer::BindParticleAttributeBlock(PtclAttributeBuffer* ptclAttributeBuffer, ParticleShader* shader, u32 numInstances)
{
#ifdef EFT_WIN
    VertexBuffer::BindExtBuffer(shader->attrWldPosBuffer,     4, sizeof(PtclAttributeBuffer),  0 * 4);
    VertexBuffer::BindExtBuffer(shader->attrSclBuffer,        4, sizeof(PtclAttributeBuffer),  4 * 4);
    VertexBuffer::BindExtBuffer(shader->attrColor0Buffer,     4, sizeof(PtclAttributeBuffer),  8 * 4);
    VertexBuffer::BindExtBuffer(shader->attrColor1Buffer,     4, sizeof(PtclAttributeBuffer), 12 * 4);
    VertexBuffer::BindExtBuffer(shader->attrTexAnimBuffer,    4, sizeof(PtclAttributeBuffer), 16 * 4);
    VertexBuffer::BindExtBuffer(shader->attrWldPosDfBuffer,   4, sizeof(PtclAttributeBuffer), 20 * 4);
    VertexBuffer::BindExtBuffer(shader->attrRotBuffer,        4, sizeof(PtclAttributeBuffer), 24 * 4);
    VertexBuffer::BindExtBuffer(shader->attrSubTexAnimBuffer, 4, sizeof(PtclAttributeBuffer), 28 * 4);
    VertexBuffer::BindExtBuffer(shader->attrEmMat0Buffer,     4, sizeof(PtclAttributeBuffer), 32 * 4);
    VertexBuffer::BindExtBuffer(shader->attrEmMat1Buffer,     4, sizeof(PtclAttributeBuffer), 36 * 4);
    VertexBuffer::BindExtBuffer(shader->attrEmMat2Buffer,     4, sizeof(PtclAttributeBuffer), 40 * 4);
#else
    VertexBuffer::BindExtBuffer(5, sizeof(PtclAttributeBuffer) * numInstances, 0, sizeof(PtclAttributeBuffer), ptclAttributeBuffer);
#endif // EFT_WIN
}

} } // namespace nw::eft
