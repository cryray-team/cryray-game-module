#include "stdafx.h"
#include "Actor.h"
#include "level.h"
#include "../xrGameAPI/ai_sounds.h"
#include "GamePersistent.h"
#include "ActorNightVision.h"
#include "../xrGameAPI/Actor/ActorEffector.h"

CNightVisionEffector::CNightVisionEffector(const shared_str& section)
{
    m_sounds.~HUD_SOUND_COLLECTION();
    m_sounds.LoadSound(section.c_str(), "snd_night_vision_on", "NightVisionOnSnd", true, SOUND_TYPE_ITEM_USING);
    m_sounds.LoadSound(section.c_str(), "snd_night_vision_off", "NightVisionOffSnd", true, SOUND_TYPE_ITEM_USING);
    m_sounds.LoadSound(section.c_str(), "snd_night_vision_idle", "NightVisionIdleSnd", true, SOUND_TYPE_ITEM_USING);
    m_sounds.LoadSound(section.c_str(), "snd_night_vision_broken", "NightVisionBrokenSnd", true, SOUND_TYPE_ITEM_USING);
}

void CNightVisionEffector::Start(const shared_str& sect, shared_str eff_sect, bool play_sound)
{
    m_section = sect;

    if (pStructCryRay->bUseShaderPNV)
        pSGameSettings->u32ShaderPNVRender = READ_IF_EXISTS(pSettings, r_u8, eff_sect, "shader_effect_range", 0);
    else
        pSGameSettings->u32ShaderPNVRender = 0;

    if (play_sound)
        PlaySounds(eStartSound);

    PlaySounds(eIdleSound);

    if (pSGameSettings->u32ShaderPNVRender == 0)
    {
        if (!Actor())
            return;

        AddEffector(Actor(), effNightvision, eff_sect);
    }
}

void CNightVisionEffector::Stop(bool play_sound)
{
    if (play_sound)
        PlaySounds(eStopSound);

    m_sounds.StopSound("NightVisionIdleSnd");

    pSGameSettings->u32ShaderPNVRender = 0;

    if (!Actor())
        return;

    CEffectorPP* pp = Actor()->Cameras().GetPPEffector((EEffectorPPType)effNightvision);

    if (pp)
        pp->Stop(100000.0f);
}

bool CNightVisionEffector::IsActive()
{
    if (pSGameSettings->u32ShaderPNVRender != 0)
        return true;

    if (!Actor())
        return false;

    CEffectorPP* pp = Actor()->Cameras().GetPPEffector((EEffectorPPType)effNightvision);
    return (pp != NULL);
}

void CNightVisionEffector::OnDisabled(CActor* pA, bool play_sound)
{
    if (play_sound)
        PlaySounds(eBrokeSound);
}

void CNightVisionEffector::PlaySounds(EPlaySounds which)
{
    if (!Actor())
        return;

    bool bPlaySoundFirstPerson = true;

    switch (which)
    {
        case eStartSound: 
        {
            m_sounds.PlaySound("NightVisionOnSnd", Actor()->Position(), Actor(), bPlaySoundFirstPerson);
        }
        break;
        case eStopSound: 
        {
            m_sounds.PlaySound("NightVisionOffSnd", Actor()->Position(), Actor(), bPlaySoundFirstPerson);
        }
        break;
        case eIdleSound: 
        {
            m_sounds.PlaySound("NightVisionIdleSnd", Actor()->Position(), Actor(), bPlaySoundFirstPerson, true);
        }
        break;
        case eBrokeSound: 
        {
            m_sounds.PlaySound("NightVisionBrokenSnd", Actor()->Position(), Actor(), bPlaySoundFirstPerson);
        }
        break;
        default: NODEFAULT;
    }
}
