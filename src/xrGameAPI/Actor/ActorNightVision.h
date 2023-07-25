#pragma once
#include "hudsound.h"
#include "../xrGameAPI/Actor/ActorEffector.h"

class CNightVisionEffector
{
    HUD_SOUND_COLLECTION m_sounds;

public:
    enum EPlaySounds
    {
        eStartSound = 0,
        eStopSound,
        eIdleSound,
        eBrokeSound
    };
    CNightVisionEffector(const shared_str& sect);
    shared_str m_section;
    void Start(const shared_str& sect, shared_str eff_sect, bool play_sound = true);
    void Stop(bool play_sound = true);
    void OnDisabled(CActor* pA, bool play_sound = true);
    void PlaySounds(EPlaySounds which);
    bool IsActive();
};
