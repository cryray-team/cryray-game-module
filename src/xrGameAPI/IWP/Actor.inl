//////////////////////////////////////////////////////////////////////////////////////////
//-' Author : ValeroK
//-' CryRay Engine x64

#pragma once

void CActor::PlayMovementAnm(shared_str name, bool wpn_aim, bool hud)
{
    bool Aiming = false;
    if (wpn_aim)
    {
        CWeapon* pWeapon = smart_cast<CWeapon*>(Actor()->inventory().ActiveItem());
        if (pWeapon && pWeapon->IsZoomed())
            Aiming = true;
    }
    int rand = ::Random.randI(5000, 10000);
    string_path anm_name;
    strconcat(sizeof(anm_name), anm_name, "camera_effects\\actor_move\\", name.c_str(), Aiming ? "_aim.anm" : ".anm");
    CAnimatorCamEffector* e = xr_new<CAnimatorCamEffector>();
    e->SetType(ECamEffectorType(rand));
    e->SetHudAffect(hud);
    e->SetCyclic(false);
    e->Start(anm_name);
    Cameras().AddCamEffector(e);
}

void CActor::PlayCamera(shared_str name, bool hud)
{
    int rand = ::Random.randI(5000, 10000);
    CAnimatorCamEffector* e = xr_new<CAnimatorCamEffector>();
    e->SetType(ECamEffectorType(rand));
    e->SetHudAffect(hud);
    e->SetCyclic(false);
    e->Start(name.c_str());
    Cameras().AddCamEffector(e);
}