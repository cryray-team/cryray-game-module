#pragma once

#include "ui/UIStatic.h"

#include "../xrGameAPI/UI/UICustomMap.h"
#include "../xrGameAPI/UI/UIGlobalMap.h"
#include "../xrGameAPI/UI/UILevelMap.h"
#include "../xrGameAPI/UI/UIMiniMap.h"

using namespace UICustomMap;
using namespace UIGlobalMap;
using namespace UILevelMap;
using namespace UIMiniMap;

class CActor;
class UIMiniMap::CUIMiniMap;

class CUIZoneMap
{
public:
    bool visible;
    bool disabled;

private:
    UIMiniMap::CUIMiniMap* m_activeMap;

    CUIStatic m_background;
    CUIStatic* m_clock_wnd;
    CUIStatic m_center;
    CUIStatic m_compass;
    CUIWindow m_clipFrame;
    CUIStatic m_Counter;
    CUITextWnd m_Counter_text;
    u8 m_current_map_idx;
    CUIStatic m_pointerDistanceText;

public:
    CUIZoneMap();
    virtual ~CUIZoneMap();

    void Init();

    void Render();
    void Update();

    bool ZoomIn();
    bool ZoomOut();

    CUIStatic& Background() { return m_background; };
    CUIWindow& MapFrame() { return m_clipFrame; };
    void SetupCurrentMap();
    void OnSectorChanged(int sector);
    void Counter_ResetClrAnimation();
    UIMiniMap::CUIMiniMap* MiniMap() { return m_activeMap; }

private:
    void SetHeading(float angle);
    void UpdateRadar(Fvector pos);
};
