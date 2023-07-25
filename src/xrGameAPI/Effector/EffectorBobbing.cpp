#include "stdafx.h"
#include "EffectorBobbing.h"

#include "actor.h"
#include "../xrGameAPI/Actor/ActorDefs.h"

#define SPEED_REMINDER 4.0f

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEffectorBobbing::CEffectorBobbing() : CEffectorCam(eCEBobbing, 10000.f)
{
    fTime = 0.0f;
    fReminderFactor = 0.0f;
    fAmplTmp = 0.0f;
    fOldAmpl = 0.0f;
    fPhaseTmp = 0.0f;
    fOldPhase = 0.0f;
    fSpeedTmp = 0.0f;
    fOldSpeed = 0.0f;
    is_limping = false;

    m_fAmplitudeFall = pCryRaySettings->r_float("bobbing_effector", "fall_amplitude");
    m_fAmplitudeSprint = pCryRaySettings->r_float("bobbing_effector", "sprint_amplitude");
    m_fAmplitudeRun = pCryRaySettings->r_float("bobbing_effector", "run_amplitude");
    m_fAmplitudeZoomRun = pCryRaySettings->r_float("bobbing_effector", "zoom_run_amplitude");
    m_fAmplitudeWalk = pCryRaySettings->r_float("bobbing_effector", "walk_amplitude");
    m_fAmplitudeZoomWalk = pCryRaySettings->r_float("bobbing_effector", "zoom_walk_amplitude");
    m_fAmplitudeLimp = pCryRaySettings->r_float("bobbing_effector", "limp_amplitude");
    m_fAmplitudeZoomLimp = pCryRaySettings->r_float("bobbing_effector", "zoom_limp_speed");
    m_fAmplitudeCrouch = pCryRaySettings->r_float("bobbing_effector", "crouch_amplitude");
    m_fAmplitudeZoomCrouch = pCryRaySettings->r_float("bobbing_effector", "zoom_crouch_amplitude");
    m_fAmplitudeSlowCrouch = pCryRaySettings->r_float("bobbing_effector", "slow_crouch_amplitude");
    m_fAmplitudeZoomSlowCrouch = pCryRaySettings->r_float("bobbing_effector", "zoom_slow_crouch_amplitude");

    m_fSpeedFall = pCryRaySettings->r_float("bobbing_effector", "fall_speed");
    m_fSpeedSprint = pCryRaySettings->r_float("bobbing_effector", "sprint_speed");
    m_fSpeedRun = pCryRaySettings->r_float("bobbing_effector", "run_speed");
    m_fSpeedZoomRun = pCryRaySettings->r_float("bobbing_effector", "zoom_run_speed");
    m_fSpeedWalk = pCryRaySettings->r_float("bobbing_effector", "walk_speed");
    m_fSpeedZoomWalk = pCryRaySettings->r_float("bobbing_effector", "zoom_walk_speed");
    m_fSpeedLimp = pCryRaySettings->r_float("bobbing_effector", "limp_speed");
    m_fSpeedZoomLimp = pCryRaySettings->r_float("bobbing_effector", "zoom_limp_speed");
    m_fSpeedCrouch = pCryRaySettings->r_float("bobbing_effector", "crouch_speed");
    m_fSpeedZoomCrouch = pCryRaySettings->r_float("bobbing_effector", "zoom_crouch_speed");
    m_fSpeedSlowCrouch = pCryRaySettings->r_float("bobbing_effector", "slow_crouch_speed");
    m_fSpeedZoomSlowCrouch = pCryRaySettings->r_float("bobbing_effector", "zoom_slow_crouch_speed");

    fAmplDelta = pCryRaySettings->r_float("bobbing_effector", "amplitude_delta");
}

CEffectorBobbing::~CEffectorBobbing() {}

void CEffectorBobbing::SetState(u32 mstate, bool limping, bool ZoomMode)
{
    dwMState = mstate;
    is_limping = limping;
    m_bZoomMode = ZoomMode;
}

BOOL CEffectorBobbing::ProcessCam(SCamEffectorInfo& info)
{
    fTime += Device.fTimeDelta;
    if (dwMState & ACTOR_DEFS::mcAnyMove || dwMState & ACTOR_DEFS::mcFall || dwMState & ACTOR_DEFS::mcJump)
    {
        if (fReminderFactor < 1.f)
            fReminderFactor += SPEED_REMINDER * Device.fTimeDelta;
        else
            fReminderFactor = 1.f;
    }
    else
    {
        if (fReminderFactor > 0.f)
            fReminderFactor -= SPEED_REMINDER * Device.fTimeDelta;
        else
            fReminderFactor = 0.f;
    }
    if (!fsimilar(fReminderFactor, 0))
    {
        Fmatrix M;
        M.identity();
        M.j.set(info.n);
        M.k.set(info.d);
        M.i.crossproduct(info.n, info.d);
        M.c.set(info.p);

        // apply footstep bobbing effect
        Fvector dangle;

        float Ampl, Speed;

        if (dwMState & ACTOR_DEFS::mcFall || dwMState & ACTOR_DEFS::mcJump)
        {
            Ampl = m_fAmplitudeFall;
            Speed = m_fSpeedFall;
        }
        else if (dwMState & ACTOR_DEFS::mcSprint)
        {
            Ampl = m_fAmplitudeSprint;
            Speed = m_fSpeedSprint;
        }
        else if (is_limping)
        {
            if (m_bZoomMode)
            {
                Ampl = m_fAmplitudeZoomLimp;
                Speed = m_fSpeedZoomLimp;
            }
            else
            {
                Ampl = m_fAmplitudeLimp;
                Speed = m_fSpeedLimp;
            }
        }
        else if ((dwMState & ACTOR_DEFS::mcCrouch) && (dwMState & ACTOR_DEFS::mcAccelerated))
        {
            if (m_bZoomMode)
            {
                Ampl = m_fAmplitudeZoomSlowCrouch;
                Speed = m_fSpeedZoomSlowCrouch;
            }
            else
            {
                Ampl = m_fAmplitudeSlowCrouch;
                Speed = m_fSpeedSlowCrouch;
            }
        }
        else if (dwMState & ACTOR_DEFS::mcCrouch)
        {
            if (m_bZoomMode)
            {
                Ampl = m_fAmplitudeZoomCrouch;
                Speed = m_fSpeedZoomCrouch;
            }
            else
            {
                Ampl = m_fAmplitudeCrouch;
                Speed = m_fSpeedCrouch;
            }
        }
        else if (dwMState & ACTOR_DEFS::mcAccelerated)
        {
            if (m_bZoomMode)
            {
                Ampl = m_fAmplitudeZoomWalk;
                Speed = m_fSpeedZoomWalk;
            }
            else
            {
                Ampl = m_fAmplitudeWalk;
                Speed = m_fSpeedWalk;
            }
        }
        else
        {
            if (m_bZoomMode)
            {
                Ampl = m_fAmplitudeZoomRun;
                Speed = m_fSpeedZoomRun;
            }
            else
            {
                Ampl = m_fAmplitudeRun;
                Speed = m_fSpeedRun;
            }
        }

        fAmplTmp = Ampl;
        fPhaseTmp = fOldPhase;
        fSpeedTmp = Speed;

        if (fSpeedTmp != fOldSpeed)
        {
            // Подберем новую фазу так, чтобы при изменении частоты положение камеры осталось прежним
            fPhaseTmp = (fOldSpeed - fSpeedTmp) * fTime + fOldPhase;
            if (fPhaseTmp > 2.0f * PI)
                fPhaseTmp = fPhaseTmp - floor(fPhaseTmp / (2.0f * PI)) * 2.0f * PI;
            else if (fPhaseTmp < 0.0f)
                fPhaseTmp = fPhaseTmp + ceil(abs(fPhaseTmp) / (2.0f * PI)) * 2.0f * PI;

            fOldSpeed = fSpeedTmp;
            fOldPhase = fPhaseTmp;
        }

        if (fAmplTmp != fOldAmpl)
        {
            // Сделаем уменьшение амплитуды плавным
            float dt = fAmplDelta * Device.fTimeDelta;
            if (fAmplTmp > fOldAmpl)
            {
                if ((fAmplTmp - fOldAmpl) > dt)
                    fAmplTmp = fOldAmpl + dt;
            }
            else if ((fOldAmpl - fAmplTmp) > dt)
                fAmplTmp = fOldAmpl - dt;

            fOldAmpl = fAmplTmp;
        }

        float ST = fTime * fSpeedTmp + fPhaseTmp;
        float A = fAmplTmp;

        float _sinA = _abs(_sin(ST) * A) * fReminderFactor;
        float _cosA = _cos(ST) * A * fReminderFactor;

        info.p.y += _sinA;
        dangle.x = _cosA;
        dangle.z = _cosA;
        dangle.y = _sinA;

        Fmatrix R;
        R.setHPB(dangle.x, dangle.y, dangle.z);

        Fmatrix mR;
        mR.mul(M, R);

        info.d.set(mR.k);
        info.n.set(mR.j);
    }
    return TRUE;
}
