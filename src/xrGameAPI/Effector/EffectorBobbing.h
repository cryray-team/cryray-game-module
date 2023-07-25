#ifndef _EFFECTOR_BOBBING_H
#define _EFFECTOR_BOBBING_H
#pragma once

#include "../xrGameAPI/Actor/ActorCameraEffector.h"
#include "../xrEngine/cameramanager.h"

class CEffectorBobbing : public CEffectorCam
{
    float fTime;
    Fvector vAngleAmplitude;
    float fYAmplitude;
    float fSpeed;

    u32 dwMState;
    float fReminderFactor;
    bool is_limping;
    bool m_bZoomMode;

    float m_fAmplitudeFall;
    float m_fAmplitudeSprint;
    float m_fAmplitudeCrouch;
    float m_fAmplitudeZoomCrouch;
    float m_fAmplitudeSlowCrouch;
    float m_fAmplitudeZoomSlowCrouch;
    float m_fAmplitudeRun;
    float m_fAmplitudeZoomRun;
    float m_fAmplitudeWalk;
    float m_fAmplitudeZoomWalk;
    float m_fAmplitudeLimp;
    float m_fAmplitudeZoomLimp;

    float m_fSpeedFall;
    float m_fSpeedSprint;
    float m_fSpeedCrouch;
    float m_fSpeedZoomCrouch;
    float m_fSpeedSlowCrouch;
    float m_fSpeedZoomSlowCrouch;
    float m_fSpeedRun;
    float m_fSpeedZoomRun;
    float m_fSpeedWalk;
    float m_fSpeedZoomWalk;
    float m_fSpeedLimp;
    float m_fSpeedZoomLimp;

    float fAmplTmp;
    float fOldAmpl;
    float fPhaseTmp;
    float fOldPhase;
    float fSpeedTmp;
    float fOldSpeed;

    float fAmplDelta;

public:
    CEffectorBobbing();
    virtual ~CEffectorBobbing();
    virtual BOOL ProcessCam(SCamEffectorInfo& info);
    void SetState(u32 st, bool limping, bool ZoomMode);
};

#endif //_EFFECTOR_BOBBING_H
