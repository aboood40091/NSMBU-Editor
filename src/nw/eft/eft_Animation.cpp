#include <nw/eft/eft_Animation.h>
#include <nw/eft/eft_ResData.h>

namespace nw { namespace eft {

f32 CalcAnimKeyFrame(KeyFrameAnim* anim, f32 frame)
{
    KeyFrameAnimKey* keys = reinterpret_cast<KeyFrameAnimKey*>(anim + 1);
    if (anim->numKeys == 1)
        return keys[0].value;

    KeyFrameAnimKey& firstKey = keys[0];
    KeyFrameAnimKey& lastKey  = keys[anim->numKeys - 1];
    f32 time = frame;

    if (anim->loop != 0)
    {
        time = fmodf(frame, lastKey.time);
        if (time <= firstKey.time)
            return firstKey.value;
    }
    else
    {
        if (time <= firstKey.time)
            return firstKey.value;

        if (lastKey.time <= time)
            return lastKey.value;
    }

    s32 startIdx = -1;
    for (u32 i = 0; i < anim->numKeys; i++)
    {
        if (time < keys[i].time) break;
        startIdx++;
    }

    KeyFrameAnimKey& secStartKey = keys[startIdx];
    KeyFrameAnimKey& secEndKey   = (startIdx + 1u != anim->numKeys) ? keys[startIdx + 1u] : keys[startIdx];

    f32 secDuration = secEndKey.time - secStartKey.time;
    if (secDuration == 0.0f)
        return secStartKey.value;

    f32 rate = (time - secStartKey.time) / secDuration;

    if (anim->interpolation == 1)
        rate = rate * rate * (3.0f - 2.0f * rate);

    return secStartKey.value * (1.0f - rate) + secEndKey.value * rate;
}

} } // namespace nw::eft
