#include "stdafx.h"
#include "MapManager.h"
#include "Level.h"
#include "../xrGame/UIGameSP.h"
#include "../xrGame/ui/UIMainIngameWnd.h"
#include "../xrGame/ui/UIMotionIcon.h"
#include "../xrGame/HUDManager.h"
#include "../xrGame/map_location.h"
#include "../xrGame/map_manager.h"
#include "../xrGame/ui/UIDialogWnd.h"
#include "../xrGame/ui/UIMotionIcon.h"

using XRay::MapManager;
extern GAME_API CUIMotionIcon* g_pMotionIcon;

void MapManager::Indicators::set(ETypeIndicator eType)
{
	switch (eType)
	{
	case ETypeIndicator::Show:		ShowIndicators(); break;
	case ETypeIndicator::Hide:		HideIndicators(); break;
	case ETypeIndicator::HideSafe:	HideIndicatorsSafe(); break;
	}
}
void MapManager::HideIndicators()
{
#if 0
    if (((CUIGameSP*)UIDialogWnd::GetGameUI().ToPointer()))
	{
        ((CUIGameSP*)UIDialogWnd::GetGameUI().ToPointer())->HideShownDialogs();
        ((CUIGameSP*)UIDialogWnd::GetGameUI().ToPointer())->ShowGameIndicators(false);
        ((CUIGameSP*)UIDialogWnd::GetGameUI().ToPointer())->ShowCrosshair(false);
	}
#endif
}
void MapManager::HideIndicatorsSafe()
{
#if 0
	if ((CUIGameSP*)UIDialogWnd::GetGameUI().ToPointer())
	{
        ((CUIGameSP*)UIDialogWnd::GetGameUI().ToPointer())->ShowGameIndicators(false);
        ((CUIGameSP*)UIDialogWnd::GetGameUI().ToPointer())->ShowCrosshair(false);
        ((CUIGameSP*)UIDialogWnd::GetGameUI().ToPointer())->OnExternalHideIndicators();
	}
#endif
}

void MapManager::ShowMinimap::set(bool bShow)
{
    CUIGameSP* GameUI = static_cast<CUIGameSP*>(HUD().GetGameUI());
	GameUI->UIMainIngameWnd->ShowZoneMap(bShow);
	if (g_pMotionIcon != nullptr)
	{
		g_pMotionIcon->bVisible = bShow;
	}
}

void MapManager::ShowIndicators()
{
    /*if (((CUIGameSP*)UIDialogWnd::GetGameUI().ToPointer()))
	{
        ((CUIGameSP*)UIDialogWnd::GetGameUI().ToPointer())->ShowGameIndicators(true);
        ((CUIGameSP*)UIDialogWnd::GetGameUI().ToPointer())->ShowCrosshair(true);
	}*/
}

void MapManager::MapAddObjectSpot(u16 id, ::System::String^ SpotType, ::System::String^ Text)
{
	TO_STRING64(spot_type, SpotType);
	TO_STRING64(text, Text);
	
	CMapLocation* ml = (::Level().MapManager().AddMapLocation(spot_type, id));
	
	if (Text->Length > 0)
		ml->SetHint(text);
}

void MapManager::MapAddObjectSpotSer(u16 id, ::System::String^ SpotType, ::System::String^ Text)
{
	TO_STRING64(spot_type, SpotType);
	TO_STRING64(text, Text);
	
	CMapLocation* ml = (::Level().MapManager().AddMapLocation(spot_type, id));
	
	if (Text->Length > 0)
		ml->SetHint(text);

	ml->SetSerializable(true);
}

void MapManager::MapChangeSpotHint(u16 id, ::System::String^ SpotType, ::System::String^ Text)
{
	TO_STRING64(spot_type, SpotType);
	TO_STRING64(text, Text);
	
	CMapLocation* ml = (::Level().MapManager().GetMapLocation(spot_type, id));
	
	if (Text->Length > 0)
		ml->SetHint(text);
}

void MapManager::MapRemoveObjectSpot(u16 id, ::System::String^ SpotType)
{
	TO_STRING64(spot_type, SpotType);
	(::Level().MapManager().RemoveMapLocation(spot_type, id));
}

bool MapManager::MapHasObjectSpot(u16 id, ::System::String^ SpotType)
{
	TO_STRING64(spot_type, SpotType);
	return (::Level().MapManager().HasMapLocation(spot_type, id));
}