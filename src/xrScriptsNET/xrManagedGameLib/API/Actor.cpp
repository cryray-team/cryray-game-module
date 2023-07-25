#include "stdafx.h"
#include "Actor.h"
#include "../xrGame/Actor.h"
#include "../xrGame/ActorCondition.h"
#include "../xrGameAPI/Actor/ActorEffector.h"
#include "../xrGame/PostprocessAnimator.h"
#include "../xrGame/HUDManager.h"

#include "../xrServerEntities/character_info.h"
#include "../xrServerEntities/specific_character.h"

extern GAME_API bool g_bDisableAllInput;

XRay::Actor::Actor(IntPtr InNativeObject)
	:EntityAlive(InNativeObject)
{
	CAST_TO_NATIVE_OBJECT(CActor, InNativeObject);

	inventory = (Inventory^)Inventory::Create(InNativeObject, Inventory::typeid);

	AddVirtualMethod("TradeStart");
	AddVirtualMethod("TradeStop");
	AddVirtualMethod("TradeSellBuyItem");
	AddVirtualMethod("TradePerformOperation");

	AddVirtualMethod("TraderGlobalAnimRequest");
	AddVirtualMethod("TraderHeadAnimRequest");
	AddVirtualMethod("TraderSoundEend");

	AddVirtualMethod("ZoneEnter");
	AddVirtualMethod("ZoneExit");

	AddVirtualMethod("UseObject");
}

XRay::Actor::Actor() : Actor(XRay::ClassRegistrator::GetFactoryTarget())
{

}

void XRay::Actor::bInputActive::set(bool bVal)
{
}

bool XRay::Actor::bInputActive::get()
{
	return false;
}

float XRay::Actor::Satiety::get()
{
	return ::Actor()->conditions().GetSatiety();
}

void XRay::Actor::Satiety::set(float fNewValue)
{
	::Actor()->conditions().ChangeSatiety(fNewValue);
}

u8  XRay::Actor::ActiveCam::get()
{
    CActor* actor = dynamic_cast<CActor*>(::Level().CurrentViewEntity());
	if (actor)
		return (u8)actor->active_cam();

	return 255;
}
void XRay::Actor::ActiveCam::set(u8 mode)
{
    CActor* actor = dynamic_cast<CActor*>(::Level().CurrentViewEntity());
	if (actor && mode <= EActorCameras::eacMaxCam)
		actor->cam_Set((EActorCameras)mode);
}

float XRay::Actor::AddCamEffector(LPCSTR fn, int id, bool cyclic, LPCSTR cb_func)
{
	CAnimatorCamEffectorScriptCB* e = new CAnimatorCamEffectorScriptCB(cb_func);
	e->SetType((ECamEffectorType)id);
	e->SetCyclic(cyclic);
	e->Start(fn);
	::Actor()->Cameras().AddCamEffector(e);

	return	e->GetAnimatorLength();
}

float XRay::Actor::AddCamEffector2(LPCSTR fn, int id, bool cyclic, LPCSTR cb_func, float cam_fov)
{
	CAnimatorCamEffectorScriptCB* e = xr_new<CAnimatorCamEffectorScriptCB>(cb_func);
	e->m_bAbsolutePositioning = true;
	e->m_fov = cam_fov;
	e->SetType((ECamEffectorType)id);
	e->SetCyclic(cyclic);
	e->Start(fn);
	::Actor()->Cameras().AddCamEffector(e);
	return e->GetAnimatorLength();
}

void XRay::Actor::AddComplexEffector(LPCSTR section, int id)
{
	AddEffector(::Actor(), id, section);
}

void XRay::Actor::RemoveCamEffector(int id)
{
	::Actor()->Cameras().RemoveCamEffector((ECamEffectorType)id);
}

void XRay::Actor::RemoveComplexEffector(int id)
{
	RemoveEffector(::Actor(), id);
}

void XRay::Actor::AddPPEffector(LPCSTR fn, int id, bool cyclic)
{
	CPostprocessAnimator* pp = xr_new<CPostprocessAnimator>(id, cyclic);
	pp->Load(fn);
	::Actor()->Cameras().AddPPEffector(pp);
}

void XRay::Actor::RemovePPEffector(int id)
{
	CPostprocessAnimator*	pp = dynamic_cast<CPostprocessAnimator*>(::Actor()->Cameras().GetPPEffector((EEffectorPPType)id));
	if (pp) pp->Stop(1.0f);
}

void XRay::Actor::SetPPEffectorFactor(int id, float f, float f_sp)
{
	CPostprocessAnimator*	pp = dynamic_cast<CPostprocessAnimator*>(::Actor()->Cameras().GetPPEffector((EEffectorPPType)id));

	if (pp) pp->SetDesiredFactor(f, f_sp);
}

void XRay::Actor::SetPPEffectorFactor2(int id, float f)
{
	CPostprocessAnimator*	pp = dynamic_cast<CPostprocessAnimator*>(::Actor()->Cameras().GetPPEffector((EEffectorPPType)id));

	if (pp) pp->SetCurrentFactor(f);
}

bool XRay::Actor::ShowWeapon::get()
{
	return psHUD_Flags.is(HUD_WEAPON_RT2);
}

void XRay::Actor::ShowWeapon::set(bool b)
{
	psHUD_Flags.set(HUD_WEAPON_RT2, b);
}

////////////////////////////////////////////////////
//			Callbacks
////////////////////////////////////////////////////
void XRay::Actor::UseObject(Object ^ pObj)
{
}

void XRay::Actor::TradeStart()
{

}

void XRay::Actor::TradeStop()
{
}

void XRay::Actor::TradeSellBuyItem()
{

}

void XRay::Actor::TradePerformOperation()
{

}

void XRay::Actor::TraderGlobalAnimRequest()
{

}

void XRay::Actor::TraderHeadAnimRequest()
{

}

void XRay::Actor::TraderSoundEend()
{

}

void XRay::Actor::ZoneEnter()
{

}

void XRay::Actor::ZoneExit()
{

}