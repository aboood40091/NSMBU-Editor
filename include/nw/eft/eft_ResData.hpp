#pragma once

#include "eft_ResData.h"
#include "eft_Shader.h"

#ifdef EFT_WIN

#if (__BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__)
#error "Windows host expected to be little-endian"
#endif

#include <cassert>

static inline void Swap16(void* buf)
{
    *(u16*)buf = __builtin_bswap16(*(u16*)buf);
}

static inline void Swap32(void* buf)
{
    *(u32*)buf = __builtin_bswap32(*(u32*)buf);
}

static inline void Swap32(void* buf, u32 count)
{
    for (u32 i = 0; i < count; i++)
        Swap32((u32*)buf + i);
}

#endif // EFT_WIN

static inline nw::eft::Header* LoadNwEftHeader(void* data, bool isBigEndian = true)
{
    nw::eft::Header* header = reinterpret_cast<nw::eft::Header*>(data);

#ifdef EFT_WIN
    if (isBigEndian)
    {
        assert(header != NULL);

        Swap32(&header->version);
        Swap32(&header->numEmitterSet);
        Swap32(&header->strTblOffs);
        Swap32(&header->textureDataTblOffs);
        Swap32(&header->textureDataTblSize);
        Swap32(&header->shaderTblOffs);
        Swap32(&header->shaderTblSize);
        Swap32(&header->keyAnimArrayTblOffs);
        Swap32(&header->keyAnimArrayTblSize);
        Swap32(&header->primitiveTblOffs);
        Swap32(&header->primitiveTblSize);
    }
#endif // EFT_WIN

    return header;
}

static inline nw::eft::ShaderTable* LoadNwEftShaderTable(void* data, bool isBigEndian = true)
{
    nw::eft::ShaderTable* shaderTbl = reinterpret_cast<nw::eft::ShaderTable*>(data);

#ifdef EFT_WIN
    if (isBigEndian)
    {
        Swap32(&shaderTbl->numShaderProg);
        Swap32(&shaderTbl->size);
        Swap32(&shaderTbl->shaderSrcOffs);
        Swap32(&shaderTbl->shaderProgOffs);
    }
#endif // EFT_WIN

    return shaderTbl;
}

static inline nw::eft::VertexShaderKey* LoadNwEftVertexShaderKey(void* data, bool isBigEndian = true)
{
    nw::eft::VertexShaderKey* vertexShaderKey = reinterpret_cast<nw::eft::VertexShaderKey*>(data);

#ifdef EFT_WIN
    if (isBigEndian)
    {
        Swap32(&vertexShaderKey->shaderUserFlag);
        Swap32(&vertexShaderKey->shaderUserSwitchFlag);
    }
#endif // EFT_WIN

    return vertexShaderKey;
}

static inline nw::eft::FragmentShaderKey* LoadNwEftFragmentShaderKey(void* data, bool isBigEndian = true)
{
    nw::eft::FragmentShaderKey* fragmentShaderKey = reinterpret_cast<nw::eft::FragmentShaderKey*>(data);

#ifdef EFT_WIN
    if (isBigEndian)
    {
        Swap16(&fragmentShaderKey->_unused0);
        Swap32(&fragmentShaderKey->shaderUserFlag);
        Swap32(&fragmentShaderKey->shaderUserSwitchFlag);
        Swap16(&fragmentShaderKey->_unused1);
    }
#endif // EFT_WIN

    return fragmentShaderKey;
}

static inline nw::eft::ShaderProgram* LoadNwEftShaderProgram(void* data, bool isBigEndian = true)
{
    nw::eft::ShaderProgram* program = reinterpret_cast<nw::eft::ShaderProgram*>(data);

#ifdef EFT_WIN
    (void)LoadNwEftVertexShaderKey(&program->vertexShaderKey, isBigEndian);
    (void)LoadNwEftFragmentShaderKey(&program->fragmentShaderKey, isBigEndian);
    //(void)LoadNwEftGeometryShaderKey(&program->geometryShaderKey, isBigEndian);

    if (isBigEndian)
    {
        Swap32(&program->binSize);
        Swap32(&program->binOffs);
    }
#endif // EFT_WIN

    return program;
}

static inline nw::eft::PrimitiveTable* LoadNwEftPrimitiveTable(void* data, bool isBigEndian = true)
{
    nw::eft::PrimitiveTable* primitiveTbl = reinterpret_cast<nw::eft::PrimitiveTable*>(data);

#ifdef EFT_WIN
    if (isBigEndian)
    {
        Swap32(&primitiveTbl->numPrimitive);
        Swap32(&primitiveTbl->size);
        Swap32(&primitiveTbl->primitiveOffs);
    }
#endif // EFT_WIN

    return primitiveTbl;
}

static inline nw::eft::PrimitiveAttrib* LoadNwEftPrimitiveAttrib(void* data, bool isBigEndian = true)
{
    nw::eft::PrimitiveAttrib* primitiveAttrib = reinterpret_cast<nw::eft::PrimitiveAttrib*>(data);

#ifdef EFT_WIN
    if (isBigEndian)
    {
        Swap32(&primitiveAttrib->count);
        Swap32(&primitiveAttrib->size);
        Swap32(&primitiveAttrib->bufferOffs);
        Swap32(&primitiveAttrib->bufferSize);
    }
#endif // EFT_WIN

    return primitiveAttrib;
}

static inline nw::eft::PrimitiveData* LoadNwEftPrimitiveData(void* data, bool isBigEndian = true)
{
    nw::eft::PrimitiveData* primitiveData = reinterpret_cast<nw::eft::PrimitiveData*>(data);

#ifdef EFT_WIN
    (void)LoadNwEftPrimitiveAttrib(&primitiveData->pos, isBigEndian);
    (void)LoadNwEftPrimitiveAttrib(&primitiveData->normal, isBigEndian);
    (void)LoadNwEftPrimitiveAttrib(&primitiveData->color, isBigEndian);
    (void)LoadNwEftPrimitiveAttrib(&primitiveData->texCoord, isBigEndian);
    (void)LoadNwEftPrimitiveAttrib(&primitiveData->index, isBigEndian);
#endif // EFT_WIN

    return primitiveData;
}

static inline nw::eft::EmitterSetData* LoadNwEftEmitterSetData(void* data, bool isBigEndian = true)
{
    nw::eft::EmitterSetData* emitterSetData = reinterpret_cast<nw::eft::EmitterSetData*>(data);

#ifdef EFT_WIN
    if (isBigEndian)
    {
        Swap32(&emitterSetData->userData);
        Swap32(&emitterSetData->nameOffs);
        Swap32(&emitterSetData->numEmitter);
        Swap32(&emitterSetData->emitterRefOffs);
    }
#endif // EFT_WIN

    return emitterSetData;
}

static inline nw::eft::EmitterTblData* LoadNwEftEmitterTblData(void* data, bool isBigEndian = true)
{
    nw::eft::EmitterTblData* emitterRef = reinterpret_cast<nw::eft::EmitterTblData*>(data);

#ifdef EFT_WIN
    if (isBigEndian)
    {
        Swap32(&emitterRef->dataOffs);
    }
#endif // EFT_WIN

    return emitterRef;
}

static inline nw::eft::TextureRes* LoadNwEftTextureRes(void* data, bool isBigEndian = true)
{
    nw::eft::TextureRes* texture = reinterpret_cast<nw::eft::TextureRes*>(data);

#ifdef EFT_WIN
    if (isBigEndian)
    {
        Swap16(&texture->width);
        Swap16(&texture->height);
        Swap32(&texture->tileMode);
        Swap32(&texture->swizzle);
        Swap32(&texture->alignment);
        Swap32(&texture->pitch);
        Swap32(&texture->numMips);
        Swap32(&texture->compSel);
        Swap32(&texture->mipOffset, 13);
        Swap32(&texture->maxLOD);
        Swap32(&texture->biasLOD);
        Swap32(&texture->originalTexFormat);
        Swap32(&texture->originalTexDataOffs);
        Swap32(&texture->originalTexDataSize);
        Swap32(&texture->cafeTexFormat);
        Swap32(&texture->cafeTexDataSize);
        Swap32(&texture->cafeTexDataOffs);
    }
#endif // EFT_WIN

    return texture;
}

static inline nw::eft::EmitterKeyAnimArray* LoadNwEftEmitterKeyAnimArray(void* data, bool isBigEndian = true)
{
    nw::eft::EmitterKeyAnimArray* keyAnimArray = reinterpret_cast<nw::eft::EmitterKeyAnimArray*>(data);

#ifdef EFT_WIN
    if (isBigEndian)
    {
        Swap32(&keyAnimArray->offset);
        Swap32(&keyAnimArray->size);
    }
#endif // EFT_WIN

    return keyAnimArray;
}

static inline nw::eft::EmitterPrimitive* LoadNwEftEmitterPrimitive(void* data, bool isBigEndian = true)
{
    nw::eft::EmitterPrimitive* primitive = reinterpret_cast<nw::eft::EmitterPrimitive*>(data);

#ifdef EFT_WIN
    if (isBigEndian)
    {
        Swap32(&primitive->idx);
    }
#endif // EFT_WIN

    return primitive;
}

static inline nw::eft::TextureEmitterData* LoadNwEftTextureEmitterData(void* data, bool isBigEndian = true)
{
    nw::eft::TextureEmitterData* texAnimParam = reinterpret_cast<nw::eft::TextureEmitterData*>(data);

#ifdef EFT_WIN
    if (isBigEndian)
    {
        Swap16(&texAnimParam->texPtnAnimPeriod);
        Swap16(&texAnimParam->texPtnAnimUsedSize);
        Swap32(&texAnimParam->uvScaleInit, 2);
        Swap32(&texAnimParam->uvShiftAnimMode, 2);
        Swap32(&texAnimParam->texIncScroll, 2);
        Swap32(&texAnimParam->texInitScroll, 2);
        Swap32(&texAnimParam->texInitScrollRandom, 2);
        Swap32(&texAnimParam->texIncScale, 2);
        Swap32(&texAnimParam->texInitScale, 2);
        Swap32(&texAnimParam->texInitScaleRandom, 2);
        Swap32(&texAnimParam->texIncRotate);
        Swap32(&texAnimParam->texInitRotate);
        Swap32(&texAnimParam->texInitRotateRandom);
    }
#endif // EFT_WIN

    return texAnimParam;
}

static inline nw::eft::anim3v4Key* LoadNwEftAnim3v4Key(void* data, bool isBigEndian = true)
{
    nw::eft::anim3v4Key* anim = reinterpret_cast<nw::eft::anim3v4Key*>(data);

#ifdef EFT_WIN
    if (isBigEndian)
    {
        Swap32(&anim->startValue);
        Swap32(&anim->startDiff);
        Swap32(&anim->endDiff);
        Swap32(&anim->time2);
        Swap32(&anim->time3);
    }
#endif // EFT_WIN

    return anim;
}

static inline nw::eft::ChildData* LoadNwEftChildData(void* data, bool isBigEndian = true)
{
    nw::eft::ChildData* childData = reinterpret_cast<nw::eft::ChildData*>(data);

#ifdef EFT_WIN
    (void)LoadNwEftEmitterPrimitive(&childData->primitive, isBigEndian);
    (void)LoadNwEftTextureRes(&childData->texture, isBigEndian);

    if (isBigEndian)
    {
        Swap32(&childData->numChildParticles);
        Swap32(&childData->startFramePercent);
        Swap32(&childData->ptclMaxLifespan);
        Swap32(&childData->emissionInterval);
        Swap32(&childData->velInheritRatio);
        Swap32(&childData->allDirVel);
        Swap32(&childData->diffusionVel, 3);
        Swap32(&childData->ptclPosRandom);
        Swap32(&childData->momentumRandom);
        Swap32(&childData->blendType);
        Swap32(&childData->meshType);
        Swap32(&childData->vertexTransformMode);
        Swap32(&childData->zBufATestType);
        Swap32(&childData->displaySideType);
        Swap32(&childData->ptclColor0, 3);
        Swap32(&childData->ptclColor1, 3);
        Swap32(&childData->primitiveColorBlend);
        Swap32(&childData->primitiveAlphaBlend);
        Swap32(&childData->ptclAlphaMid);
        Swap32(&childData->ptclAlphaEnd);
        Swap32(&childData->ptclAlphaStart);
        Swap32(&childData->scaleInheritRatio);
        Swap32(&childData->ptclEmitScale, 2);
        Swap32(&childData->ptclScaleRandom);
        Swap32(&childData->rotationMode);
        Swap32(&childData->ptclRotate, 3);
        Swap32(&childData->ptclRotateRandom, 3);
        Swap32(&childData->angularVelocity, 3);
        Swap32(&childData->angularVelocityRandom, 3);
        Swap32(&childData->rotInertia);
        Swap32(&childData->rotBasis, 2);
        Swap32(&childData->gravity, 3);
        Swap32(&childData->alphaAnimTime3);
        Swap32(&childData->alphaAnimTime2);
        Swap32(&childData->scaleAnimTime1);
        Swap32(&childData->ptclScaleEnd, 2);
        Swap16(&childData->texPtnAnimNum);
        Swap32(&childData->uvScaleInit, 2);
        Swap16(&childData->texPtnAnimPeriod);
        Swap16(&childData->texPtnAnimUsedSize);
        Swap32(&childData->fragmentColorMode);
        Swap32(&childData->fragmentAlphaMode);
        Swap32(&childData->airResist);
        Swap32(&childData->shaderParam01, 2);
        Swap32(&childData->fragmentSoftEdgeFadeDist);
        Swap32(&childData->fragmentSoftEdgeVolume);
        Swap32(&childData->shaderUserFlag);
        Swap32(&childData->shaderUserSwitchFlag);
    }
#endif // EFT_WIN

    return childData;
}

static inline nw::eft::FieldRandomData* LoadNwEftFieldRandomData(void* data, bool isBigEndian = true)
{
    nw::eft::FieldRandomData* fieldRandomData = reinterpret_cast<nw::eft::FieldRandomData*>(data);

#ifdef EFT_WIN
    if (isBigEndian)
    {
        Swap32(&fieldRandomData->period);
        Swap32(&fieldRandomData->randomVelScale, 3);
    }
#endif // EFT_WIN

    return fieldRandomData;
}

static inline nw::eft::FieldMagnetData* LoadNwEftFieldMagnetData(void* data, bool isBigEndian = true)
{
    nw::eft::FieldMagnetData* fieldMagnetData = reinterpret_cast<nw::eft::FieldMagnetData*>(data);

#ifdef EFT_WIN
    if (isBigEndian)
    {
        Swap32(&fieldMagnetData->strength);
        Swap32(&fieldMagnetData->pos, 3);
        Swap32(&fieldMagnetData->flags);
    }
#endif // EFT_WIN

    return fieldMagnetData;
}

static inline nw::eft::FieldSpinData* LoadNwEftFieldSpinData(void* data, bool isBigEndian = true)
{
    nw::eft::FieldSpinData* fieldSpinData = reinterpret_cast<nw::eft::FieldSpinData*>(data);

#ifdef EFT_WIN
    if (isBigEndian)
    {
        Swap32(&fieldSpinData->angle);
        Swap32(&fieldSpinData->axis);
        Swap32(&fieldSpinData->diffusionVel);
    }
#endif // EFT_WIN

    return fieldSpinData;
}

static inline nw::eft::FieldCollisionData* LoadNwEftFieldCollisionData(void* data, bool isBigEndian = true)
{
    nw::eft::FieldCollisionData* fieldCollisionData = reinterpret_cast<nw::eft::FieldCollisionData*>(data);

#ifdef EFT_WIN
    if (isBigEndian)
    {
        Swap16(&fieldCollisionData->collisionType);
        Swap16(&fieldCollisionData->coordSystem);
        Swap32(&fieldCollisionData->y);
        Swap32(&fieldCollisionData->bounceRate);
    }
#endif // EFT_WIN

    return fieldCollisionData;
}

static inline nw::eft::FieldConvergenceData* LoadNwEftFieldConvergenceData(void* data, bool isBigEndian = true)
{
    nw::eft::FieldConvergenceData* fieldConvergenceData = reinterpret_cast<nw::eft::FieldConvergenceData*>(data);

#ifdef EFT_WIN
    if (isBigEndian)
    {
        Swap32(&fieldConvergenceData->pos, 3);
        Swap32(&fieldConvergenceData->strength);
    }
#endif // EFT_WIN

    return fieldConvergenceData;
}

static inline nw::eft::FieldPosAddData* LoadNwEftFieldPosAddData(void* data, bool isBigEndian = true)
{
    nw::eft::FieldPosAddData* fieldPosAddData = reinterpret_cast<nw::eft::FieldPosAddData*>(data);

#ifdef EFT_WIN
    if (isBigEndian)
    {
        Swap32(&fieldPosAddData->posAdd, 3);
    }
#endif // EFT_WIN

    return fieldPosAddData;
}

static inline void* LoadNwEftFieldData(void* data, u16 fieldFlags, bool isBigEndian = true)
{
    void* fieldData = data;

#ifdef EFT_WIN
    if (fieldFlags & 0x01) fieldData = LoadNwEftFieldRandomData     (fieldData, isBigEndian) + 1;
    if (fieldFlags & 0x02) fieldData = LoadNwEftFieldMagnetData     (fieldData, isBigEndian) + 1;
    if (fieldFlags & 0x04) fieldData = LoadNwEftFieldSpinData       (fieldData, isBigEndian) + 1;
    if (fieldFlags & 0x08) fieldData = LoadNwEftFieldCollisionData  (fieldData, isBigEndian) + 1;
    if (fieldFlags & 0x10) fieldData = LoadNwEftFieldConvergenceData(fieldData, isBigEndian) + 1;
    if (fieldFlags & 0x20) fieldData = LoadNwEftFieldPosAddData     (fieldData, isBigEndian) + 1;
#endif // EFT_WIN

    return data;
}

static inline nw::eft::FluctuationData* LoadNwEftFluctuationData(void* data, bool isBigEndian = true)
{
    nw::eft::FluctuationData* fluctuationData = reinterpret_cast<nw::eft::FluctuationData*>(data);

#ifdef EFT_WIN
    if (isBigEndian)
    {
        Swap32(&fluctuationData->amplitude);
        Swap32(&fluctuationData->frequency);
        Swap32(&fluctuationData->enableRandom);
    }
#endif // EFT_WIN

    return fluctuationData;
}

static inline nw::eft::StripeData* LoadNwEftStripeData(void* data, bool isBigEndian = true)
{
    nw::eft::StripeData* stripeData = reinterpret_cast<nw::eft::StripeData*>(data);

#ifdef EFT_WIN
    if (isBigEndian)
    {
        Swap32(&stripeData->type);
        Swap32(&stripeData->crossType);
        Swap32(&stripeData->connectionType);
        Swap32(&stripeData->textureType);
        Swap32(&stripeData->numSliceHistory);
        Swap32(&stripeData->numDivisions);
        Swap32(&stripeData->alphaStart);
        Swap32(&stripeData->alphaEnd);
        Swap32(&stripeData->sliceHistInterval);
        Swap32(&stripeData->sliceInterpolation);
        Swap32(&stripeData->dirInterpolation);
    }
#endif // EFT_WIN

    return stripeData;
}

static inline nw::eft::EmitterData* LoadNwEftEmitterData(void* data, bool isBigEndian = true)
{
    nw::eft::ComplexEmitterData* cdata = reinterpret_cast<nw::eft::ComplexEmitterData*>(data);

#ifdef EFT_WIN
    (void)LoadNwEftTextureRes(&cdata->textures[0], isBigEndian);
    (void)LoadNwEftTextureRes(&cdata->textures[1], isBigEndian);
    (void)LoadNwEftEmitterKeyAnimArray(&cdata->keyAnimArray, isBigEndian);
    (void)LoadNwEftEmitterPrimitive(&cdata->primitive, isBigEndian);

    if (isBigEndian)
    {
        Swap32(&cdata->type);
        Swap32(&cdata->flags);
        Swap32(&cdata->seed);
        Swap32(&cdata->userData);
        Swap32(&cdata->callbackID);
        Swap32(&cdata->nameOffs);

        assert(cdata->type == nw::eft::EmitterType_Simple ||
               cdata->type == nw::eft::EmitterType_Complex);
    }

    (void)LoadNwEftTextureEmitterData(&cdata->texAnimParam[0], isBigEndian);
    (void)LoadNwEftTextureEmitterData(&cdata->texAnimParam[1], isBigEndian);
    (void)LoadNwEftAnim3v4Key(&cdata->alphaAnim, isBigEndian);

    if (isBigEndian)
    {
        Swap32(&cdata->rotationMode);
        Swap32(&cdata->ptclFollowType);
        Swap32(&cdata->fragmentColorMode);
        Swap32(&cdata->fragmentAlphaMode);
        Swap32(&cdata->_bitForUnusedFlag);
        Swap32(&cdata->displaySideType);
        Swap32(&cdata->momentumRandom);
        Swap32(&cdata->animMatrixSRT, 3 * 4);
        Swap32(&cdata->animMatrixRT, 3 * 4);
        Swap32(&cdata->emitterScale, 3);
        Swap32(&cdata->emitterRotate, 3);
        Swap32(&cdata->emitterTranslate, 3);
        Swap32(&cdata->emitterRotateRandom, 3);
        Swap32(&cdata->emitterTranslateRandom, 3);
        Swap32(&cdata->blendType);
        Swap32(&cdata->zBufATestType);
        Swap32(&cdata->emitFunction);
        Swap32(&cdata->volumeScale, 3);
        Swap32(&cdata->arcStartAngle);
        Swap32(&cdata->arcLength);
        Swap32(&cdata->volumeFillRatio);
        Swap32(&cdata->sphereLatitude);
        Swap32(&cdata->sphereLatitudeDir, 3);
        Swap32(&cdata->lineCenter);
        Swap32(&cdata->emissionShapeScale, 3);
        Swap32(&cdata->emitterColor0, 4);
        Swap32(&cdata->emitterColor1, 4);
        Swap32(&cdata->emitterAlpha);
        Swap32(&cdata->emitSameDistanceUnit);
        Swap32(&cdata->emitSameDistanceMax);
        Swap32(&cdata->emitSameDistanceMin);
        Swap32(&cdata->emitSameDistanceThreshold);
        Swap32(&cdata->emissionRate);
        Swap32(&cdata->startFrame);
        Swap32(&cdata->endFrame);
        Swap32(&cdata->emitInterval);
        Swap32(&cdata->emitIntervalRandom);
        Swap32(&cdata->allDirVel);
        Swap32(&cdata->dirVel);
        Swap32(&cdata->dirVelRandom);
        Swap32(&cdata->dir, 3);
        Swap32(&cdata->dispersionAngle);
        Swap32(&cdata->diffusionVel, 3);
        Swap32(&cdata->airResist);
        Swap32(&cdata->gravity, 3);
        Swap32(&cdata->yDiffusionVel);
        Swap32(&cdata->ptclPosRandom);
        Swap32(&cdata->ptclMaxLifespan);
        Swap32(&cdata->ptclLifespanRandom);
        Swap32(&cdata->meshType);
        Swap32(&cdata->vertexTransformMode);
        Swap32(&cdata->rotBasis, 2);
        Swap32(&cdata->cameraOffset);
        Swap32(&cdata->ptclColorSrc, 2);
        Swap32(&cdata->ptclColorTbl, 2 * 3 * 4);
        Swap32(&cdata->colorTime2, 2);
        Swap32(&cdata->colorTime3, 2);
        Swap32(&cdata->colorTime4, 2);
        Swap32(&cdata->colorNumRepetition, 2);
        Swap32(&cdata->colorRandomStart, 2);
        Swap32(&cdata->colorScaleFactor);
        Swap32(&cdata->textureColorBlend);
        Swap32(&cdata->primitiveColorBlend);
        Swap32(&cdata->textureAlphaBlend);
        Swap32(&cdata->primitiveAlphaBlend);
        Swap32(&cdata->scaleAnimTime2);
        Swap32(&cdata->scaleAnimTime3);
        Swap32(&cdata->ptclScaleRandom);
        Swap32(&cdata->ptclEmitScale, 2);
        Swap32(&cdata->ptclScaleStart, 2);
        Swap32(&cdata->ptclScaleStartDiff, 2);
        Swap32(&cdata->ptclScaleEndDiff, 2);
        Swap32(&cdata->ptclRotate, 3);
        Swap32(&cdata->ptclRotateRandom, 3);
        Swap32(&cdata->angularVelocity, 3);
        Swap32(&cdata->angularVelocityRandom, 3);
        Swap32(&cdata->rotInertia);
        Swap32(&cdata->fadeAlphaStep);
        Swap32(&cdata->shaderParam01, 2);
        Swap32(&cdata->fragmentSoftEdgeFadeDist);
        Swap32(&cdata->fragmentSoftEdgeVolume);
        Swap32(&cdata->shaderUserFlag);
        Swap32(&cdata->shaderUserSwitchFlag);
    }

    if (cdata->type == nw::eft::EmitterType_Complex)
    {
        if (isBigEndian)
        {
            Swap32(&cdata->childFlags);
            Swap16(&cdata->fieldFlags);
            Swap16(&cdata->fluctuationFlags);
            Swap16(&cdata->stripeFlags);
            Swap16(&cdata->childDataOffs);
            Swap16(&cdata->fieldDataOffs);
            Swap16(&cdata->fluctuationDataOffs);
            Swap16(&cdata->stripeDataOffs);
            Swap32(&cdata->dataSize);
        }

        if (cdata->childFlags & 1)
        {
            assert(cdata->childDataOffs == sizeof(nw::eft::ComplexEmitterData));
            (void)LoadNwEftChildData(cdata + 1);
        }

        if (cdata->fieldFlags != 0)
        {
            assert(cdata->fieldDataOffs >= sizeof(nw::eft::ComplexEmitterData));
            (void)LoadNwEftFieldData((u8*)cdata + cdata->fieldDataOffs, cdata->fieldFlags);
        }

        if (cdata->fluctuationFlags & 1)
        {
            assert(cdata->fluctuationDataOffs >= sizeof(nw::eft::ComplexEmitterData));
            (void)LoadNwEftFluctuationData((u8*)cdata + cdata->fluctuationDataOffs);
        }

        if (cdata->vertexTransformMode == nw::eft::VertexTransformMode_Stripe ||
            cdata->vertexTransformMode == nw::eft::VertexTransformMode_Complex_Stripe)
        {
            assert(cdata->stripeDataOffs >= sizeof(nw::eft::ComplexEmitterData));
            (void)LoadNwEftStripeData((u8*)cdata + cdata->stripeDataOffs);
        }
    }
#endif // EFT_WIN

    return cdata;
}

static inline nw::eft::KeyFrameAnimKey* LoadNwEftKeyFrameAnimKey(void* data, bool isBigEndian = true)
{
    nw::eft::KeyFrameAnimKey* key = reinterpret_cast<nw::eft::KeyFrameAnimKey*>(data);

#ifdef EFT_WIN
    if (isBigEndian)
    {
        Swap32(&key->time);
        Swap32(&key->value);
    }
#endif // EFT_WIN

    return key;
}

static inline nw::eft::KeyFrameAnim* LoadNwEftKeyFrameAnim(void* data, bool isBigEndian = true)
{
    nw::eft::KeyFrameAnim* anim = reinterpret_cast<nw::eft::KeyFrameAnim*>(data);

#ifdef EFT_WIN
    if (isBigEndian)
    {
        Swap32(&anim->numKeys);
        Swap32(&anim->interpolation);
        Swap32(&anim->animValIdx);
        Swap32(&anim->loop);
        Swap32(&anim->nextOffs);
    }

    nw::eft::KeyFrameAnimKey* key = reinterpret_cast<nw::eft::KeyFrameAnimKey*>(anim + 1);
    for (u32 i = 0; i < anim->numKeys; i++)
    {
        (void)LoadNwEftKeyFrameAnimKey(key, isBigEndian);
        key = key + 1;
    }
#endif // EFT_WIN

    return anim;
}

static inline nw::eft::KeyFrameAnimArray* LoadNwEftKeyFrameAnimArray(void* data, bool isBigEndian = true)
{
    nw::eft::KeyFrameAnimArray* keyAnimArray = reinterpret_cast<nw::eft::KeyFrameAnimArray*>(data);

#ifdef EFT_WIN
    if (isBigEndian)
    {
        Swap32(&keyAnimArray->numAnim);
    }

    nw::eft::KeyFrameAnim* anim = reinterpret_cast<nw::eft::KeyFrameAnim*>(keyAnimArray + 1);
    for (u32 i = 0; i < keyAnimArray->numAnim; i++)
    {
        (void)LoadNwEftKeyFrameAnim(anim, isBigEndian);
        anim = reinterpret_cast<nw::eft::KeyFrameAnim*>((uintptr_t)anim + anim->nextOffs);
    }
#endif // EFT_WIN

    return keyAnimArray;
}
