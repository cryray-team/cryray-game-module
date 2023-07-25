#include "stdafx.h"
#include "detector_af_react.h"
#include "Actor.h"
#include "torch.h"
#include "script_engine.h"
#include "ai_space.h"

CDetectorAfReact::CDetectorAfReact() {}

CDetectorAfReact::~CDetectorAfReact() {}

BOOL CDetectorAfReact::net_Spawn(CSE_Abstract* DC) { return (inherited::net_Spawn(DC)); }

void CDetectorAfReact::Load(LPCSTR section) { inherited::Load(section); }

void CDetectorAfReact::net_Destroy() { inherited::net_Destroy(); }

void CDetectorAfReact::shedule_Update(u32 dt) { inherited::shedule_Update(dt); }

void CDetectorAfReact::UpdateCL() { inherited::UpdateCL(); }

void CDetectorAfReact::OnH_A_Chield() { inherited::OnH_A_Chield(); }

void CDetectorAfReact::OnH_B_Independent(bool just_before_destroy)
{
    inherited::OnH_B_Independent(just_before_destroy);
}

void CDetectorAfReact::renderable_Render() { inherited::renderable_Render(); }

bool CDetectorAfReact::UseBy(CEntityAlive* entity_alive)
{
    //CDetectorCheckerScript();

    bool used = inherited::UseBy(entity_alive);
    return used;
}

bool CDetectorAfReact::Empty() const
{
    CTorch* flashlight = Actor()->GetCurrentTorch();
    if (!flashlight)
        return false;

    return inherited::Empty();
}
