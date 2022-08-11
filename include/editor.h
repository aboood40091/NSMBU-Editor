#pragma once

#include <gfx/lyr/rio_Drawable.h>
#include <task/rio_Task.h>

#include <nw/math.h>

class Editor : public rio::ITask, public rio::lyr::IDrawable
{
public:
    Editor();

    void renderForeground(const rio::lyr::DrawInfo& drawInfo);
    void renderBackground(const rio::lyr::DrawInfo& drawInfo);

private:
    void initEftSystem_();
    void calcEftSystem_() const;
    void drawEftSystem_(const nw::math::MTX44& proj, const nw::math::MTX34& view, const nw::math::VEC3& camPos, f32 zNear, f32 zFar) const;
    void calcEftSystemNextEmitterSet_();

    void prepare_() override;
    void exit_() override;
    void calc_() override;

    u8* mPtclFile;
    u32 mTimer;
    bool mIsRunning;
};
