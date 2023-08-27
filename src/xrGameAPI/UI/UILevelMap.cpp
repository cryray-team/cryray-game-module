#include "stdafx.h"

#include "level.h"
#include "map_location.h"
#include "map_manager.h"
#include "map_spot.h"
#include "UI/UIMapWnd.h"

#include "UILevelMap.h"
#include "xr_input.h"

namespace UILevelMap
{
    CUILevelMap::CUILevelMap(CUIMapWnd* p)
    {
        m_mapWnd = p;
        Show(false);
    }

    CUILevelMap::~CUILevelMap() {}

    void CUILevelMap::Draw()
    {
        if (MapWnd())
        {
            float gmz = MapWnd()->GlobalMap()->GetCurrentZoom().x;
            for (WINDOW_LIST_it it = m_ChildWndList.begin(); m_ChildWndList.end() != it; ++it)
            {
                CMapSpot* sp = smart_cast<CMapSpot*>((*it));
                if (sp)
                {
                    if (sp->m_bScale)
                    {
                        Fvector2 sz = sp->m_originSize;
                        float k = gmz;

                        if (gmz > sp->m_scale_bounds.y)
                            k = sp->m_scale_bounds.y;
                        else if (gmz < sp->m_scale_bounds.x)
                            k = sp->m_scale_bounds.x;

                        sz.mul(k);
                        sp->SetWndSize(sz);
                    }
                    else if (sp->m_scale_bounds.x > 0.0f)
                        sp->SetVisible(sp->m_scale_bounds.x < gmz);
                }
            }
        }
        inherited::Draw();
    }

    void CUILevelMap::Init_internal(const shared_str& name, CInifile& pLtx, const shared_str& sect_name, LPCSTR sh_name)
    {
        inherited::Init_internal(name, pLtx, sect_name, sh_name);
        Fvector4 tmp = pGameIni->r_fvector4(MapName(), "global_rect");

        tmp.x *= UI().get_current_kx();
        tmp.z *= UI().get_current_kx();
        m_GlobalRect.set(tmp.x, tmp.y, tmp.z, tmp.w);

    #ifdef DEBUG
        float kw = m_GlobalRect.width() / BoundRect().width();
        float kh = m_GlobalRect.height() / BoundRect().height();

        if (FALSE == fsimilar(kw, kh, EPS_L))
        {
            Msg(" --incorrect global rect definition for map [%s]  kw=%f kh=%f", *MapName(), kw, kh);
            Msg(" --try x2=%f or  y2=%f", m_GlobalRect.x1 + kh * BoundRect().width(),
                m_GlobalRect.y1 + kw * BoundRect().height());
        }
    #endif
    }

    void CUILevelMap::UpdateSpots()
    {
        DetachAll();

        Frect _r;
        GetAbsoluteRect(_r);

        if (FALSE == MapWnd()->ActiveMapRect().intersected(_r))
            return;

        Locations& ls = Level().MapManager().Locations();
        Locations_it it = ls.begin();
        Locations_it it_e = ls.end();

        for (u32 idx = 0; it != it_e; ++it, ++idx)
        {
            if ((*it).actual && MapName() == (*it).location->GetLevelName())
            {
                (*it).location->UpdateLevelMap(this);
            }
        }
    }

    Frect CUILevelMap::CalcWndRectOnGlobal()
    {
        Frect res;
        UIGlobalMap::CUIGlobalMap* globalMap = MapWnd()->GlobalMap();

        res.lt = globalMap->ConvertRealToLocal(GlobalRect().lt, false);
        res.rb = globalMap->ConvertRealToLocal(GlobalRect().rb, false);
        res.add(globalMap->GetWndPos().x, globalMap->GetWndPos().y);

        return res;
    }

    void CUILevelMap::Show(bool status) { inherited::Show(status); }

    void CUILevelMap::Update()
    {
        UIGlobalMap::CUIGlobalMap* w = MapWnd()->GlobalMap();
        Frect rect;
        Fvector2 tmp;

        tmp = w->ConvertRealToLocal(GlobalRect().lt, false);
        rect.lt = tmp;
        tmp = w->ConvertRealToLocal(GlobalRect().rb, false);
        rect.rb = tmp;

        SetWndRect(rect);

        inherited::Update();

        if (m_bCursorOverWindow)
        {
            VERIFY(m_dwFocusReceiveTime >= 0);
            if (Device.dwTimeGlobal > (m_dwFocusReceiveTime + 500))
            {
                if (fsimilar(MapWnd()->GlobalMap()->GetCurrentZoom().x, MapWnd()->GlobalMap()->GetMinZoom(), EPS_L))
                    MapWnd()->ShowHintStr(this, MapName().c_str());
                else
                    MapWnd()->HideHint(this);
            }
        }
    }

    bool CUILevelMap::OnMouseAction(float x, float y, EUIMessages mouse_action)
    {
        if (inherited::OnMouseAction(x, y, mouse_action))
            return true;

        if (MapWnd()->GlobalMap()->Locked())
            return true;

        if (mouse_action == WINDOW_MOUSE_MOVE && (FALSE == pInput->iGetAsyncBtnState(0)))
        {
            if (MapWnd())
            {
                MapWnd()->Hint(MapName());
                return true;
            }
        }
        return false;
    }

    void CUILevelMap::SendMessage(CUIWindow* pWnd, s16 msg, void* pData)
    {
        inherited::SendMessage(pWnd, msg, pData);

        if (msg == MAP_SHOW_HINT)
        {
            CMapSpot* sp = smart_cast<CMapSpot*>(pWnd);
            VERIFY(sp);
            if (sp)
            {
                MapWnd()->ShowHintSpot(sp);
            }
        }
        else if (msg == MAP_HIDE_HINT)
        {
            MapWnd()->HideHint(pWnd);
        }
        else if (msg == MAP_SELECT_SPOT)
        {
            MapWnd()->SpotSelected(pWnd);
        }
        else if (msg == MAP_SELECT_SPOT2)
        {
            MapWnd()->ActivatePropertiesBox(pWnd);
        }
    }

    void CUILevelMap::OnFocusLost()
    {
        inherited::OnFocusLost();
        MapWnd()->HideHint(this);
    }
} 