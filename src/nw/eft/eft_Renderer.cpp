#include <nw/eft/eft_Config.h>
#include <nw/eft/eft_Emitter.h>
#include <nw/eft/eft_EmitterSet.h>
#include <nw/eft/eft_Particle.h>
#include <nw/eft/eft_Primitive.h>
#include <nw/eft/eft_Renderer.h>
#include <nw/eft/eft_Shader.h>
#include <nw/eft/eft_System.h>
#include <nw/eft/eft_UniformBlock.h>

#include <cstdlib>

namespace nw { namespace eft {

s32 Renderer::ComparePtclViewZ(const void* a, const void* b)
{
    const PtclViewZ* ptcl_a = static_cast<const PtclViewZ*>(a);
    const PtclViewZ* ptcl_b = static_cast<const PtclViewZ*>(b);

    if (ptcl_a->z < 0.0f && ptcl_b->z < 0.0f)
    {
        if (ptcl_a->z < ptcl_b->z)
            return -1;
    }
    else
    {
        if (ptcl_a->z > ptcl_b->z)
            return -1;
    }

    return 1;
}

Renderer::Renderer(Heap* argHeap, System* argSystem, const Config& config)
    : system(argSystem)
{
    depthBufferTextureOffset.x = 0.0f;
    depthBufferTextureOffset.y = 0.0f;
    depthBufferTextureScale.x = 1.0f;
    depthBufferTextureScale.y = 1.0f;
    frameBufferTextureOffset.x = 0.0f;
    frameBufferTextureOffset.y = 0.0f;
    frameBufferTextureScale.x = 1.0f;
    frameBufferTextureScale.y = 1.0f;

    heap = argHeap;

    depthBufferTexture = EFT_NATIVE_TEXTURE_NONE;
    frameBufferTexture = EFT_NATIVE_TEXTURE_NONE;

    currentParticleType = PtclType_Max;
    shaderType = ShaderType_Normal;
    renderVisibilityFlags = 0x3F;

    math::VEC3* pos = static_cast<nw::math::VEC3*>(vbPos.AllocateVertexBuffer(argHeap, sizeof(math::VEC3) * 4, 3));
    pos[0].x = -0.5f;
    pos[0].y =  0.5f;
    pos[0].z =  0.0f;
    pos[1].x = -0.5f;
    pos[1].y = -0.5f;
    pos[1].z =  0.0f;
#ifdef EFT_WIN
    pos[2].x =  0.5f;
    pos[2].y =  0.5f;
    pos[2].z =  0.0f;
    pos[3].x =  0.5f;
    pos[3].y = -0.5f;
    pos[3].z =  0.0f;
#else
    pos[2].x =  0.5f;
    pos[2].y = -0.5f;
    pos[2].z =  0.0f;
    pos[3].x =  0.5f;
    pos[3].y =  0.5f;
    pos[3].z =  0.0f;
    vbPos.Invalidate();
#endif // EFT_WIN

    u32* index = static_cast<u32*>(vbIndex.AllocateVertexBuffer(argHeap, sizeof(u32) * 4, 1));
    index[0] = 0;
    index[1] = 1;
#ifdef EFT_WIN
    index[2] = 3;
    index[3] = 2;
#else
    index[2] = 2;
    index[3] = 3;
    vbIndex.Invalidate();
#endif // EFT_WIN

    primitive = NULL;

    u32 doubleBufferSize = config.doubleBufferSize;
    doubleBufferSize += config.numParticleMax *  0xCC;
    doubleBufferSize += config.numEmitterMax  * 0x114;

    doubleBuffer.Initialize(heap, doubleBufferSize);
}

void Renderer::BeginRender(const math::MTX44& proj, const math::MTX34& view, const math::VEC3& cameraWorldPos, f32 zNear, f32 zFar)
{
    renderContext.SetupCommonState();

    this->view = math::MTX44(view);

    math::MTX34 billboard34;
    math::MTX34::Transpose(&billboard34, &view);

    billboard.m[0][0] = billboard34.m[0][0];
    billboard.m[0][1] = billboard34.m[0][1];
    billboard.m[0][2] = billboard34.m[0][2];
    billboard.m[0][3] = 0.0f;
    billboard.m[1][0] = billboard34.m[1][0];
    billboard.m[1][1] = billboard34.m[1][1];
    billboard.m[1][2] = billboard34.m[1][2];
    billboard.m[1][3] = 0.0f;
    billboard.m[2][0] = billboard34.m[2][0];
    billboard.m[2][1] = billboard34.m[2][1];
    billboard.m[2][2] = billboard34.m[2][2];
    billboard.m[2][3] = 0.0f;
    billboard.m[3][0] = 0.0f;
    billboard.m[3][1] = 0.0f;
    billboard.m[3][2] = 0.0f;
    billboard.m[3][3] = 1.0f;

    eyeVec.x = billboard34.m[0][2];
    eyeVec.y = billboard34.m[1][2];
    eyeVec.z = billboard34.m[2][2];

    eyePos = cameraWorldPos;

    // Why? Couldn't you have used this->view?
    math::MTX44 view44;
    view44.m[0][0] = view.m[0][0];
    view44.m[0][1] = view.m[0][1];
    view44.m[0][2] = view.m[0][2];
    view44.m[0][3] = view.m[0][3];
    view44.m[1][0] = view.m[1][0];
    view44.m[1][1] = view.m[1][1];
    view44.m[1][2] = view.m[1][2];
    view44.m[1][3] = view.m[1][3];
    view44.m[2][0] = view.m[2][0];
    view44.m[2][1] = view.m[2][1];
    view44.m[2][2] = view.m[2][2];
    view44.m[2][3] = view.m[2][3];
    view44.m[3][0] = 0.0f;
    view44.m[3][1] = 0.0f;
    view44.m[3][2] = 0.0f;
    view44.m[3][3] = 1.0f;

    math::MTX44::Concat(&viewProj, &proj , &view44);

    viewUniformBlock = static_cast<ViewUniformBlock*>(AllocFromDoubleBuffer(sizeof(ViewUniformBlock)));
    if (viewUniformBlock != NULL)
    {
#ifdef EFT_WIN
        // Hardcoded, for now

        viewUniformBlock->viewMat = math::MTX44(1, 0, 0,  0,
                                                0, 1, 0, -1,
                                                0, 0, 1, -161.97659,
                                                0, 0, 0,  1);
        viewUniformBlock->vpMat = math::MTX44(0.0092, 0,       0,      0,
                                              0,      0.01289, 0,     -0.01289,
                                              0,      0,      -1.0E-5, 0.00162,
                                              0,      0,       0,      1);
        viewUniformBlock->bldMat = math::MTX44::Identity();
        viewUniformBlock->eyeVec.x = 0.0f;
        viewUniformBlock->eyeVec.y = 0.0f;
        viewUniformBlock->eyeVec.z = 1.0f;
        viewUniformBlock->eyeVec.w = 0.0f;
        viewUniformBlock->eyePos.x = 0.0f;
        viewUniformBlock->eyePos.y = 1.0f;
        viewUniformBlock->eyePos.z = 161.97659f;
        viewUniformBlock->eyePos.w = 0.0f;
        viewUniformBlock->depthBufferTexMat.xy() = depthBufferTextureScale;
        viewUniformBlock->depthBufferTexMat.zw() = depthBufferTextureOffset;
        viewUniformBlock->frameBufferTexMat.xy() = frameBufferTextureScale;
        viewUniformBlock->frameBufferTexMat.zw() = frameBufferTextureOffset;
        viewUniformBlock->viewParam.x = 1.0f;
        viewUniformBlock->viewParam.y = 100000.0f;
        viewUniformBlock->viewParam.z = 0.0f;
        viewUniformBlock->viewParam.w = 0.0f;
#else
        viewUniformBlock->viewMat = this->view;
        viewUniformBlock->vpMat = viewProj;
        viewUniformBlock->bldMat = billboard;
        viewUniformBlock->eyeVec.xyz() = eyeVec;
        viewUniformBlock->eyeVec.w = 0.0f;
        viewUniformBlock->eyePos.xyz() = eyePos;
        viewUniformBlock->eyePos.w = 0.0f;
        viewUniformBlock->depthBufferTexMat.xy() = depthBufferTextureScale;
        viewUniformBlock->depthBufferTexMat.zw() = depthBufferTextureOffset;
        viewUniformBlock->frameBufferTexMat.xy() = frameBufferTextureScale;
        viewUniformBlock->frameBufferTexMat.zw() = frameBufferTextureOffset;
        viewUniformBlock->viewParam.x = zNear;
        viewUniformBlock->viewParam.y = zFar;
        viewUniformBlock->viewParam.z = 0.0f;
        viewUniformBlock->viewParam.w = 0.0f;

        GX2EndianSwap(viewUniformBlock, sizeof(ViewUniformBlock));
        GX2Invalidate(GX2_INVALIDATE_CPU_UNIFORM_BLOCK, viewUniformBlock, sizeof(ViewUniformBlock));
#endif // EFT_WIN
    }
}

bool Renderer::SetupParticleShaderAndVertex(ParticleShader* shader, MeshType meshType, Primitive* primitive)
{
    shader->Bind();

    {
        const ViewUniformBlock* viewUniformBlock = this->viewUniformBlock;
        shader->vertexViewUniformBlock.BindUniformBlock(viewUniformBlock);
        shader->fragmentViewUniformBlock.BindUniformBlock(viewUniformBlock);
    }

    if (meshType == MeshType_Particle)
    {
        vbPos.BindBuffer(shader->attrPosBuffer, sizeof(math::VEC3) * 4, sizeof(math::VEC3));
        if (shader->attrIndexBuffer != 0xFFFFFFFF)
            vbIndex.BindBuffer(shader->attrIndexBuffer, sizeof(u32) * 4, sizeof(u32));

#ifdef EFT_WIN
        primitiveMode = Drawer::TRIANGLE_STRIP;
#else
        primitiveMode = Drawer::QUADS;
#endif // EFT_WIN

        this->primitive = NULL;
    }
    else
    {
        if (primitive == NULL || primitive->numIndex == 0)
            return false;

        this->primitive = primitive;
        if (!primitive->initialized)
            return false;

        {
            if (shader->attrPosBuffer != 0xFFFFFFFF && primitive->vbPos.buffer != NULL)
                primitive->vbPos.BindBuffer(shader->attrPosBuffer, primitive->vbPos.bufferSize, sizeof(math::VEC3));
        }

        {
            if (shader->attrTexCoordBuffer != 0xFFFFFFFF && primitive->vbTexCoord.buffer != NULL)
                primitive->vbTexCoord.BindBuffer(shader->attrTexCoordBuffer, primitive->vbTexCoord.bufferSize, sizeof(math::VEC2));
        }

        {
            if (shader->attrColorBuffer != 0xFFFFFFFF && primitive->vbColor.buffer != NULL)
                primitive->vbColor.BindBuffer(shader->attrColorBuffer, primitive->vbColor.bufferSize, sizeof(ut::Color4f));
        }

        u32 attrNormalBuffer = shader->attrNormalBuffer;
        if (attrNormalBuffer != 0xFFFFFFFF)
        {
            if (attrNormalBuffer != 0xFFFFFFFF && primitive->vbNormal.buffer != NULL)
                primitive->vbNormal.BindBuffer(attrNormalBuffer, primitive->vbNormal.bufferSize, sizeof(math::VEC3));
        }

        primitiveMode = Drawer::TRIANGLES;
    }

    return true;
}

void Renderer::RequestParticle(const EmitterInstance* emitter, ParticleShader* shader, bool isChild, bool flushCache, void* argData)
{
    const SimpleEmitterData* data = emitter->data;

    u32 numParticles = emitter->numParticles;
    const TextureRes* texture0 = &data->textures[0];
    const TextureRes* texture1 = &data->textures[1];
    CustomShaderCallBackID callbackID = static_cast<CustomShaderCallBackID>(data->shaderUserSetting);
    ZBufATestType zBufATestType = data->zBufATestType;
    BlendType blendType = data->blendType;
    DisplaySideType displaySideType = data->displaySideType;
    MeshType meshType = data->meshType;

    if (isChild)
    {
        const ChildData* childData = emitter->GetChildData();

        numParticles = emitter->numChildParticles;
        texture0 = &childData->texture;
        texture1 = NULL;
        callbackID = static_cast<CustomShaderCallBackID>(childData->shaderUserSetting);
        zBufATestType = childData->zBufATestType;
        blendType = childData->blendType;
        displaySideType = childData->displaySideType;
        meshType = childData->meshType;
    }

    if (numParticles == 0)
        return;

    renderContext.SetupZBufATest(zBufATestType);
    renderContext.SetupBlendType(blendType);
    renderContext.SetupDisplaySideType(displaySideType);

    renderContext.SetupTexture(texture0, TextureSlot_0, shader->fragmentSamplerLocations[0]);

    if (texture1 != NULL && texture1->initialized != 0)
        renderContext.SetupTexture(texture1, TextureSlot_1, shader->fragmentSamplerLocations[1]);
    else
        renderContext.SetupTexture((const TextureRes*)NULL, TextureSlot_1, (FragmentTextureLocation){ 0u });

    if (depthBufferTexture != EFT_NATIVE_TEXTURE_NONE && shader->fragmentDepthBufferSamplerLocation.location != 0xFFFFFFFF)
        renderContext.SetupTexture(depthBufferTexture, TextureSlot_Depth_Buffer, shader->fragmentDepthBufferSamplerLocation);

    if (frameBufferTexture != EFT_NATIVE_TEXTURE_NONE && shader->fragmentFrameBufferSamplerLocation.location != 0xFFFFFFFF)
        renderContext.SetupTexture(frameBufferTexture, TextureSlot_Frame_Buffer, shader->fragmentFrameBufferSamplerLocation);

    if (system->GetCustomShaderRenderStateSetCallback(callbackID) != NULL)
    {
        RenderStateSetArg arg = {
            .emitter = emitter,
            .renderer = this,
            .flushCache = flushCache,
            .argData = argData,
        };
        system->GetCustomShaderRenderStateSetCallback(callbackID)(arg);
    }

#ifdef EFT_WIN
    GLuint gl_vbo;
    glGenBuffers(1, &gl_vbo);
#endif // EFT_WIN

    if (!isChild && data->flags & 0x200)
    {
        u32 i = 0;

        PtclViewZ* sortedPtcls = static_cast<PtclViewZ*>(AllocFromDoubleBuffer(sizeof(PtclViewZ) * numParticles));
        if (sortedPtcls != NULL)
        {
            PtclInstance* ptcl = emitter->particleHead;
            while (ptcl != NULL)
            {
                sortedPtcls[i].ptcl = ptcl;
                sortedPtcls[i].z = view.m[2][0] * ptcl->worldPos.x + view.m[2][1] * ptcl->worldPos.y + view.m[2][2] * ptcl->worldPos.z + view.m[2][3];
                sortedPtcls[i].idx = i;

                ptcl = ptcl->next; i++;
            }

            qsort(sortedPtcls, i, sizeof(PtclViewZ), ComparePtclViewZ);

            for (u32 j = 0; j < i; j++)
            {
#ifdef EFT_WIN
                glBindBuffer(GL_ARRAY_BUFFER, gl_vbo);
                glBufferData(GL_ARRAY_BUFFER, sizeof(PtclAttributeBuffer), &emitter->ptclAttributeBuffer[sortedPtcls[j].idx], GL_STATIC_DRAW);
#endif // EFT_WIN

                BindParticleAttributeBlock(&emitter->ptclAttributeBuffer[sortedPtcls[j].idx], shader, 1);

                if (meshType == MeshType_Primitive && primitive != NULL)
                    Drawer::DrawElements(primitiveMode, primitive->numIndex, primitive->vbIndex.buffer);

                else
                    Drawer::DrawArrays(primitiveMode, 4);
            }
        }
    }
    else
    {
        u32 numDrawParticle;
        PtclAttributeBuffer* ptclAttributeBuffer;

        if (!isChild)
        {
            numDrawParticle = emitter->numDrawParticle;
            ptclAttributeBuffer = emitter->ptclAttributeBuffer;
        }

        else
        {
            numDrawParticle = emitter->numDrawChildParticle;
            ptclAttributeBuffer = emitter->childPtclAttributeBuffer;
        }

#ifdef EFT_WIN
        glBindBuffer(GL_ARRAY_BUFFER, gl_vbo);
        glBufferData(GL_ARRAY_BUFFER, numDrawParticle * sizeof(PtclAttributeBuffer), ptclAttributeBuffer, GL_STATIC_DRAW);
#endif // EFT_WIN

        BindParticleAttributeBlock(ptclAttributeBuffer, shader, numDrawParticle);

        if (meshType == MeshType_Primitive && primitive != NULL)
            Drawer::DrawElementsInstanced(primitiveMode, primitive->numIndex, primitive->vbIndex.buffer, numDrawParticle);

        else
            Drawer::DrawArraysInstanced(primitiveMode, 4, numDrawParticle);
    }

#ifdef EFT_WIN
    glDeleteBuffers(1, &gl_vbo);
#endif // EFT_WIN
}

void Renderer::EntryChildParticleSub(const EmitterInstance* emitter, bool flushCache, void* argData)
{
    ParticleShader* shader = emitter->childShader[shaderType];
    if (shader == NULL
        || emitter->childPtclAttributeBuffer == NULL
        || emitter->childEmitterDynamicUniformBlock == NULL
        || !SetupParticleShaderAndVertex(shader, emitter->GetChildData()->meshType, emitter->childPrimitive))
    {
        return;
    }

    {
        const EmitterStaticUniformBlock* emitterStaticUniformBlock = emitter->childEmitterStaticUniformBlock;
        shader->vertexEmitterStaticUniformBlock.BindUniformBlock(emitterStaticUniformBlock);
        shader->fragmentEmitterStaticUniformBlock.BindUniformBlock(emitterStaticUniformBlock);
    }

    shader->vertexEmitterDynamicUniformBlock.BindUniformBlock(emitter->childEmitterDynamicUniformBlock);

    shader->EnableInstanced();
    RequestParticle(emitter, shader, true, flushCache, argData);
    shader->DisableInstanced();
}

void Renderer::EntryParticleSub(const EmitterInstance* emitter, bool flushCache, void* argData)
{
    ParticleShader* shader = emitter->shader[shaderType];
    if (shader == NULL
        || emitter->ptclAttributeBuffer == NULL
        || emitter->emitterDynamicUniformBlock == NULL
        || !SetupParticleShaderAndVertex(shader, emitter->data->meshType, emitter->primitive))
    {
        return;
    }

    {
        const EmitterStaticUniformBlock* emitterStaticUniformBlock = emitter->emitterStaticUniformBlock;
        shader->vertexEmitterStaticUniformBlock.BindUniformBlock(emitterStaticUniformBlock);
        shader->fragmentEmitterStaticUniformBlock.BindUniformBlock(emitterStaticUniformBlock);
    }

    shader->vertexEmitterDynamicUniformBlock.BindUniformBlock(emitter->emitterDynamicUniformBlock);

    shader->EnableInstanced();
    RequestParticle(emitter, shader, false, flushCache, argData);
    shader->DisableInstanced();
}

void Renderer::EntryParticle(EmitterInstance* emitter, bool flushCache, void* argData)
{
    if (viewUniformBlock == NULL)
        return;

    CustomShaderDrawOverrideCallback callback = system->GetCustomShaderDrawOverrideCallback(static_cast<CustomShaderCallBackID>(emitter->data->shaderUserSetting));
    ShaderDrawOverrideArg arg = {
        .emitter = emitter,
        .renderer = this,
        .flushCache = flushCache,
        .argData = argData,
    };

    if (!(emitter->controller->renderVisibilityFlags & renderVisibilityFlags))
        return;

    bool stripe = false;
    if (emitter->data->vertexTransformMode == VertexTransformMode_Stripe
        || emitter->data->vertexTransformMode == VertexTransformMode_Complex_Stripe)
    {
        EntryStripe(emitter, flushCache, argData);
        stripe = true;
    }

    if (emitter->data->type == EmitterType_Complex && emitter->HasChild())
    {
        const ComplexEmitterData* cdata = emitter->GetComplexEmitterData();
        CustomShaderDrawOverrideCallback childCallback = system->GetCustomShaderDrawOverrideCallback(static_cast<CustomShaderCallBackID>(emitter->GetChildData()->shaderUserSetting));

        if (cdata->childFlags & 0x1000 && emitter->numDrawChildParticle != 0 && emitter->childPtclAttributeBuffer != NULL)
        {
            currentParticleType = PtclType_Child;

            if (childCallback != NULL)
                childCallback(arg);

            else
                EntryChildParticleSub(emitter, flushCache, argData);
        }

        if (cdata->displayParent != 0 && !stripe && emitter->numDrawParticle != 0 && emitter->ptclAttributeBuffer != NULL)
        {
            currentParticleType = PtclType_Complex;

            if (callback != NULL)
                callback(arg);

            else
                EntryParticleSub(emitter, flushCache, argData);
        }

        if (!(cdata->childFlags & 0x1000) && emitter->numDrawChildParticle != 0 && emitter->childPtclAttributeBuffer != NULL)
        {
            currentParticleType = PtclType_Child;

            if (childCallback != NULL)
                childCallback(arg);

            else
                EntryChildParticleSub(emitter, flushCache, argData);
        }
    }
    else if (!stripe)
    {
        currentParticleType = emitter->data->type == EmitterType_Simple ? PtclType_Simple
                                                                        : PtclType_Complex;

        if (emitter->data->displayParent != 0 && emitter->numDrawParticle != 0 && emitter->ptclAttributeBuffer != NULL)
        {
            if (callback != NULL)
                callback(arg);

            else
                EntryParticleSub(emitter, flushCache, argData);
        }
    }

    currentParticleType = PtclType_Max;
}

void Renderer::EndRender()
{
}

} } // namespace nw::eft
