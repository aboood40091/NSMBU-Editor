#include <nw/eft/eft_EmitterComplex.h>
#include <nw/eft/eft_EmitterSet.h>
#include <nw/eft/eft_Renderer.h>
#include <nw/eft/eft_System.h>
#include <nw/eft/eft_UniformBlock.h>

namespace nw { namespace eft {

void EmitterComplexCalc::CalcStripe(EmitterInstance* emitter, PtclInstance* ptcl, const StripeData* stripeData, const ComplexEmitterData* data, CpuCore core)
{
    s32 counter = (s32)ptcl->counter - 1;

    PtclStripe* stripe = ptcl->stripe;
    if (stripe == NULL)
        return;

    PtclStripeSliceHistory* currentSlice = &stripe->queue[stripe->queueRear];

    if (data->stripeFlags & 1)
    {
        currentSlice->pos = ptcl->pos;
        math::MTX34::Copy(&currentSlice->emitterMatrixSRT, &math::MTX34::Identity());
    }
    else
    {
        f32 sliceInterpolation = stripeData->sliceInterpolation;
        if (counter > 2 && stripeData->numSliceHistory > 3 && sliceInterpolation < 1.0f && stripe->queueRear != stripe->queueFront)
        {
            s32 prevIdx = stripe->queueRear - 1;
            if (prevIdx < 0)
                prevIdx = stripeData->numSliceHistory - 1;

            s32 prev2Idx = prevIdx - 1;
            if (prev2Idx < 0)
                prev2Idx = stripeData->numSliceHistory - 1;

            math::VEC3 diff0;
            math::VEC3::Subtract(&diff0, &ptcl->worldPos, &stripe->pos0);
            math::VEC3::Scale(&diff0, &diff0, sliceInterpolation);
            math::VEC3::Add(&stripe->pos0, &stripe->pos0, &diff0);

            math::VEC3 diff1;
            math::VEC3::Subtract(&diff1, &stripe->pos0, &stripe->pos1);
            math::VEC3::Scale(&diff1, &diff1, sliceInterpolation);
            math::VEC3::Add(&stripe->pos1, &stripe->pos1, &diff1);

            stripe->queue[prev2Idx].pos = stripe->pos1;

            math::VEC3 diff2;
            math::VEC3::Subtract(&diff2, &ptcl->worldPos, &stripe->pos1);
            math::VEC3::Scale(&diff2, &diff2, 0.7f);
            math::VEC3::Add(&stripe->queue[prevIdx].pos, &stripe->pos1, &diff2);

            currentSlice->pos = ptcl->worldPos;
        }
        else
        {
            stripe->pos0 = (stripe->pos1 = (currentSlice->pos = ptcl->worldPos));
        }

        currentSlice->emitterMatrixSRT = emitter->matrixSRT;
    }

    currentSlice->scale = ptcl->scale.x * emitter->emitterSet->ptclEffectiveScale.x;

    if (stripe->queueRear != stripe->queueFront)
    {
        s32 prevIdx = stripe->queueRear - 1;
        if (prevIdx < 0)
            prevIdx = stripeData->numSliceHistory - 1;

        PtclStripeSliceHistory* prevSlice = &stripe->queue[prevIdx];

        if (counter < 2)
        {
            math::VEC3::Subtract(&stripe->currentSliceDir, &currentSlice->pos, &prevSlice->pos);
            if (stripe->currentSliceDir.Magnitude() > 0.0f)
                stripe->currentSliceDir.Normalize();
        }
        else
        {
            math::VEC3 posDiff;
            math::VEC3::Subtract(&posDiff, &currentSlice->pos, &prevSlice->pos);
            if (posDiff.Magnitude() > 0.0f)
                posDiff.Normalize();

            math::VEC3 diff;
            math::VEC3::Subtract(&diff, &posDiff, &stripe->currentSliceDir);
            math::VEC3::Scale(&diff, &diff, stripeData->dirInterpolation);
            math::VEC3::Add(&stripe->currentSliceDir, &stripe->currentSliceDir, &diff);
            if (stripe->currentSliceDir.Magnitude() > 0.0f)
                stripe->currentSliceDir.Normalize();
        }

        currentSlice->dir = stripe->currentSliceDir;

        if (stripeData->type == 2)
        {
            currentSlice->outer.x = currentSlice->emitterMatrixSRT.m[0][1];
            currentSlice->outer.y = currentSlice->emitterMatrixSRT.m[1][1];
            currentSlice->outer.z = currentSlice->emitterMatrixSRT.m[2][1];
        }
        else
        {
            math::VEC3 outer = (math::VEC3){ currentSlice->emitterMatrixSRT.m[0][1],
                                             currentSlice->emitterMatrixSRT.m[1][1],
                                             currentSlice->emitterMatrixSRT.m[2][1] };
            math::VEC3::CrossProduct(&outer, &outer, &stripe->currentSliceDir);
            if (outer.Magnitude() > 0.0f)
                outer.Normalize();

            currentSlice->outer = outer;
        }
    }

    if (++stripe->queueRear >= stripeData->numSliceHistory)
        stripe->queueRear = 0;

    if (stripe->queueRear == stripe->queueFront
        && ++stripe->queueFront >= stripeData->numSliceHistory)
        stripe->queueFront = 0;

    if (++stripe->queueCount >= stripeData->numSliceHistory)
        stripe->queueCount = stripeData->numSliceHistory;

    stripe->emitterMatrixSRT = emitter->matrixSRT;
    stripe->counter++;
}

void EmitterComplexCalc::EmitChildParticle(EmitterInstance* emitter, PtclInstance* ptcl, CpuCore core, const ChildData* childData)
{
    s32 counter = (s32)ptcl->counter - 1;
    if (counter < ((ptcl->lifespan - 1) * childData->startFramePercent / 100))
        return;

    if (ptcl->childEmitCounter >= childData->emissionInterval || (childData->emissionInterval == 0 && childData->ptclMaxLifespan == 1))
    {
        if (ptcl->childPreCalcCounter > 0.0f)
        {
            f32 time = emitter->counter - ptcl->childPreCalcCounter + ptcl->childEmitLostTime;
            if (childData->emissionInterval != 0)
                time /= childData->emissionInterval;

            if (ptcl->childEmitLostTime >= childData->emissionInterval)
                ptcl->childEmitLostTime -= childData->emissionInterval;

            ptcl->childEmitLostTime += emitter->counter - ptcl->childPreCalcCounter - (s32)time;
        }

        mSys->AddPtclAdditionList(ptcl, core);

        ptcl->childEmitCounter = 0.0f;
        ptcl->childPreCalcCounter = emitter->counter;
    }
    else
    {
        ptcl->childEmitCounter += emitter->emissionSpeed;
    }
}

void EmitterComplexCalc::CalcComplex(EmitterInstance* emitter, PtclInstance* ptcl, CpuCore core)
{
    const ComplexEmitterData* data = static_cast<const ComplexEmitterData*>(emitter->data);

    if (data->vertexTransformMode == VertexTransformMode_Stripe)
    {
        const StripeData* stripeData = reinterpret_cast<const StripeData*>((u32)data + data->stripeDataOffs);
        CalcStripe(emitter, ptcl, stripeData, data, core);
    }

    if (data->childFlags & 1)
    {
        const ChildData* childData = reinterpret_cast<const ChildData*>(data + 1);
        EmitChildParticle(emitter, ptcl, core, childData);
    }
}

u32 EmitterComplexCalc::CalcParticle(EmitterInstance* emitter, CpuCore core, bool noCalcBehavior, bool noMakePtclAttributeBuffer)
{
    System* system = emitter->emitterSet->system;
    VertexTransformMode vertexTransformMode = emitter->data->vertexTransformMode;

    if (!noMakePtclAttributeBuffer
        && vertexTransformMode != VertexTransformMode_Stripe
        && vertexTransformMode != VertexTransformMode_Complex_Stripe)
    {
        Renderer** const renderers = system->renderers;

        emitter->ptclAttributeBuffer = static_cast<PtclAttributeBuffer*>(renderers[core]->AllocFromDoubleBuffer(sizeof(PtclAttributeBuffer) * emitter->numParticles));
        if (emitter->ptclAttributeBuffer == NULL)
            return 0;

        emitter->emitterDynamicUniformBlock = static_cast<EmitterDynamicUniformBlock*>(renderers[core]->AllocFromDoubleBuffer(sizeof(EmitterDynamicUniformBlock)));
        if (emitter->emitterDynamicUniformBlock == NULL)
        {
            emitter->ptclAttributeBuffer = NULL;
            return 0;
        }

        math::VEC3 emitterSetColor = emitter->emitterSet->color.rgb();
        emitterSetColor.x *= emitter->data->colorScaleFactor;
        emitterSetColor.y *= emitter->data->colorScaleFactor;
        emitterSetColor.z *= emitter->data->colorScaleFactor;

        emitter->emitterDynamicUniformBlock->emitterColor0.x = emitterSetColor.x * emitter->anim[11];
        emitter->emitterDynamicUniformBlock->emitterColor0.y = emitterSetColor.y * emitter->anim[12];
        emitter->emitterDynamicUniformBlock->emitterColor0.z = emitterSetColor.z * emitter->anim[13];
        emitter->emitterDynamicUniformBlock->emitterColor0.w = emitter->emitterSet->color.a * emitter->anim[14] * emitter->fadeAlpha;

        emitter->emitterDynamicUniformBlock->emitterColor1.x = emitterSetColor.x * emitter->anim[19];
        emitter->emitterDynamicUniformBlock->emitterColor1.y = emitterSetColor.y * emitter->anim[20];
        emitter->emitterDynamicUniformBlock->emitterColor1.z = emitterSetColor.z * emitter->anim[21];
        emitter->emitterDynamicUniformBlock->emitterColor1.w = emitter->emitterSet->color.a * emitter->anim[14] * emitter->fadeAlpha;

#ifndef EFT_WIN
        GX2EndianSwap(emitter->emitterDynamicUniformBlock, sizeof(EmitterDynamicUniformBlock));
#endif // EFT_WIN
    }
    else
    {
        emitter->ptclAttributeBuffer = NULL;
        emitter->emitterDynamicUniformBlock = NULL;
    }

    emitter->numDrawParticle = 0;

    const ComplexEmitterData* data = static_cast<const ComplexEmitterData*>(emitter->data);

    CustomActionParticleCalcCallback callback1 = mSys->GetCurrentCustomActionParticleCalcCallback(emitter);
    CustomActionParticleMakeAttributeCallback callback2 = mSys->GetCurrentCustomActionParticleMakeAttributeCallback(emitter);

    PtclInstance* ptcl = emitter->particleHead;
    bool reversed = false;

    if (emitter->data->flags & 0x400)
    {
        ptcl = emitter->particleTail;
        reversed = true;
    }

    for (; ptcl != NULL; ptcl = reversed ? ptcl->prev : ptcl->next)
    {
        if (ptcl->data == NULL)
            continue;

        if (!noCalcBehavior)
        {
            if (ptcl->lifespan <= (s32)ptcl->counter || (ptcl->lifespan == 1 && ptcl->counter != 0.0f))
            {
                PtclStripe* stripe = ptcl->stripe;
                if (stripe != NULL)
                {
                    const StripeData* stripeData = reinterpret_cast<const StripeData*>((u32)data + data->stripeDataOffs);

                    if (stripe->queueFront == stripe->queueRear)
                        RemoveParticle(emitter, ptcl, core);

                    else
                    {
                        if (++stripe->queueFront >= stripeData->numSliceHistory)
                            stripe->queueFront = 0;

                        stripe->queueCount--;
                        stripe->emitterMatrixSRT = emitter->matrixSRT;

                        ptcl->texAnimParam[0].scroll.x += data->texAnimParam[0].texIncScroll.x;
                        ptcl->texAnimParam[0].scroll.y += data->texAnimParam[0].texIncScroll.y;
                        ptcl->texAnimParam[0].scale.x  += data->texAnimParam[0].texIncScale.x;
                        ptcl->texAnimParam[0].scale.y  += data->texAnimParam[0].texIncScale.y;
                        ptcl->texAnimParam[0].rotate   += data->texAnimParam[0].texIncRotate;

                        ptcl->texAnimParam[1].scroll.x += data->texAnimParam[1].texIncScroll.x;
                        ptcl->texAnimParam[1].scroll.y += data->texAnimParam[1].texIncScroll.y;
                        ptcl->texAnimParam[1].scale.x  += data->texAnimParam[1].texIncScale.x;
                        ptcl->texAnimParam[1].scale.y  += data->texAnimParam[1].texIncScale.y;
                        ptcl->texAnimParam[1].rotate   += data->texAnimParam[1].texIncRotate;

                        emitter->numDrawParticle++;
                    }

                    stripe->counter++;
                }
                else
                {
                    RemoveParticle(emitter, ptcl, core);
                }

                continue;
            }

            CalcComplexParticleBehavior(emitter, ptcl, core);
            CalcComplex(emitter, ptcl, core);
        }

        if (callback1 != NULL)
        {
            ParticleCalcArg arg = {
                .emitter = emitter,
                .ptcl = ptcl,
                .core = core,
                .noCalcBehavior = noCalcBehavior,
            };
            callback1(arg);
        }

        if (!noMakePtclAttributeBuffer
            && vertexTransformMode != VertexTransformMode_Stripe
            && vertexTransformMode != VertexTransformMode_Complex_Stripe)
        {
            MakeParticleAttributeBuffer(&emitter->ptclAttributeBuffer[emitter->numDrawParticle], ptcl, emitter->shaderAvailableAttribFlg, emitter->data->cameraOffset);
            ptcl->ptclAttributeBuffer = &emitter->ptclAttributeBuffer[emitter->numDrawParticle++];

            if (callback2 != NULL)
            {
                ParticleMakeAttrArg arg = {
                    .emitter = emitter,
                    .ptcl = ptcl,
                    .core = core,
                    .noCalcBehavior = noCalcBehavior,
                };
                callback2(arg);
            }
        }
    }

    if (data->vertexTransformMode == VertexTransformMode_Stripe)
        system->renderers[core]->MakeStripeAttributeBlock(emitter);

    emitter->isCalculated = true;
    return emitter->numDrawParticle;
}

u32 EmitterComplexCalc::CalcChildParticle(EmitterInstance* emitter, CpuCore core, bool noCalcBehavior, bool noMakePtclAttributeBuffer)
{
    if (!noMakePtclAttributeBuffer)
    {
        Renderer** const renderers = emitter->emitterSet->system->renderers;

        emitter->childPtclAttributeBuffer = static_cast<PtclAttributeBuffer*>(renderers[core]->AllocFromDoubleBuffer(sizeof(PtclAttributeBuffer) * emitter->numChildParticles));
        if (emitter->childPtclAttributeBuffer == NULL)
            return 0;

        emitter->childEmitterDynamicUniformBlock = static_cast<EmitterDynamicUniformBlock*>(renderers[core]->AllocFromDoubleBuffer(sizeof(EmitterDynamicUniformBlock)));
        if (emitter->childEmitterDynamicUniformBlock == NULL)
        {
            emitter->ptclAttributeBuffer = NULL; // NOT childPtclAttributeBuffer... bug?
            return 0;
        }

        math::VEC3 emitterSetColor = emitter->emitterSet->color.rgb();
        emitterSetColor.x *= emitter->data->colorScaleFactor;
        emitterSetColor.y *= emitter->data->colorScaleFactor;
        emitterSetColor.z *= emitter->data->colorScaleFactor;

        emitter->childEmitterDynamicUniformBlock->emitterColor0.x = emitterSetColor.x * emitter->anim[11];
        emitter->childEmitterDynamicUniformBlock->emitterColor0.y = emitterSetColor.y * emitter->anim[12];
        emitter->childEmitterDynamicUniformBlock->emitterColor0.z = emitterSetColor.z * emitter->anim[13];
        emitter->childEmitterDynamicUniformBlock->emitterColor0.w = emitter->emitterSet->color.a * emitter->anim[14] * emitter->fadeAlpha;

        emitter->childEmitterDynamicUniformBlock->emitterColor1.x = emitterSetColor.x * emitter->anim[19];
        emitter->childEmitterDynamicUniformBlock->emitterColor1.y = emitterSetColor.y * emitter->anim[20];
        emitter->childEmitterDynamicUniformBlock->emitterColor1.z = emitterSetColor.z * emitter->anim[21];
        emitter->childEmitterDynamicUniformBlock->emitterColor1.w = emitter->emitterSet->color.a * emitter->anim[14] * emitter->fadeAlpha;

#ifndef EFT_WIN
        GX2EndianSwap(emitter->childEmitterDynamicUniformBlock, sizeof(EmitterDynamicUniformBlock));
#endif // EFT_WIN
    }
    else
    {
        emitter->childPtclAttributeBuffer = NULL;
        emitter->childEmitterDynamicUniformBlock = NULL;
    }

    emitter->numDrawChildParticle = 0;

    const ComplexEmitterData* data = static_cast<const ComplexEmitterData*>(emitter->data);

    CustomActionParticleCalcCallback callback1 = mSys->GetCurrentCustomActionParticleCalcCallback(emitter);
    CustomActionParticleMakeAttributeCallback callback2 = mSys->GetCurrentCustomActionParticleMakeAttributeCallback(emitter);

    for (PtclInstance* ptcl = emitter->childParticleHead; ptcl != NULL; ptcl = ptcl->next)
    {
        if (ptcl->data == NULL)
            continue;

        if (!noCalcBehavior)
        {
            if (ptcl->lifespan <= (s32)ptcl->counter || (ptcl->lifespan == 1 && ptcl->counter != 0.0f))
            {
                RemoveParticle(emitter, ptcl, core);
                continue;
            }

            if (data->childFlags & 0x40)
            {
                ptcl->matrixSRT = emitter->matrixSRT;
                ptcl->matrixRT = emitter->matrixRT;
            }

            CalcChildParticleBehavior(emitter, ptcl);
        }

        if (callback1 != NULL)
        {
            ParticleCalcArg arg = {
                .emitter = emitter,
                .ptcl = ptcl,
                .core = core,
                .noCalcBehavior = noCalcBehavior,
            };
            callback1(arg);
        }

        if (!noMakePtclAttributeBuffer)
        {
            MakeParticleAttributeBuffer(&emitter->childPtclAttributeBuffer[emitter->numDrawChildParticle], ptcl, emitter->childShaderAvailableAttribFlg, 0.0f);
            ptcl->ptclAttributeBuffer = &emitter->childPtclAttributeBuffer[emitter->numDrawChildParticle++];

            if (callback2 != NULL)
            {
                ParticleMakeAttrArg arg = {
                    .emitter = emitter,
                    .ptcl = ptcl,
                    .core = core,
                    .noCalcBehavior = noCalcBehavior,
                };
                callback2(arg);
            }
        }
    }

    emitter->isCalculated = true;
    return emitter->numDrawChildParticle;
}

} } // namespace nw::eft
