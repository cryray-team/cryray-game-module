#pragma once
#include "../xrGameAPI/Actor/ActorCameraEffector.h"

namespace PSEUDOGIGANT_STEP_EFFECTOR
{
    class CPseudogigantStepEffector : public CEffectorCam
    {
        typedef CEffectorCam inherited;

        float total;
        float max_amp;
        float period_number;
        float power;

    public:
        CPseudogigantStepEffector(float time, float amp, float periods, float power);
        virtual BOOL ProcessCam(SCamEffectorInfo& info);
    };
}

namespace PSEUDOGIGANT_JUMPER_STEP_EFFECTOR
{
    class CPseudoGigantJumperStepEffector : public CEffectorCam
    {
        typedef CEffectorCam inherited;

        float total;
        float max_amp;
        float period_number;
        float power;

    public:
        CPseudoGigantJumperStepEffector(float time, float amp, float periods, float power);
        virtual BOOL ProcessCam(SCamEffectorInfo& info);
    };
}