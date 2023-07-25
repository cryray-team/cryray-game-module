#include "stdafx.h"
#include "level.h"
#include "map_location.h"
#include "map_manager.h"
#include "map_spot.h"
#include "UIGlobalMap.h"
#include "ui/UIMapWnd.h"
#include "../../xrEngine/xr_input.h" //remove me !!!

namespace UIGlobalMap
{
    bool CUIGlobalMap::OnMouseAction(float x, float y, EUIMessages mouse_action)
    {
        if (inherited::OnMouseAction(x, y, mouse_action))
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

    CUIGlobalMap::CUIGlobalMap(CUIMapWnd* pMapWnd)
    {
        m_mapWnd = pMapWnd;
        m_minZoom = 1.f;
        Show(false);
    }

    CUIGlobalMap::~CUIGlobalMap() {}

    void CUIGlobalMap::Initialize() { Init_internal("global_map", *pGameIni, "global_map", "hud\\default"); }

    void CUIGlobalMap::Init_internal(const shared_str& name, CInifile& pLtx, const shared_str& sect_name, LPCSTR sh_name)
    {
        inherited::Init_internal(name, pLtx, sect_name, sh_name);
        //	Fvector2 size = CUIStatic::GetWndSize();
        SetMaxZoom(pLtx.r_float(m_name, "max_zoom"));
    }

    void CUIGlobalMap::Update()
    {
        for (WINDOW_LIST_it it = m_ChildWndList.begin(); m_ChildWndList.end() != it; ++it)
        {
            CUICustomMap* m = dynamic_cast<CUICustomMap*>(*it);
            if (!m)
                continue;
            m->DetachAll();
        }
        inherited::Update();
    }

    void CUIGlobalMap::ClipByVisRect()
    {
        Frect r = GetWndRect();
        Frect clip = WorkingArea();
        if (r.x2 < clip.width())
            r.x1 += clip.width() - r.x2;
        if (r.y2 < clip.height())
            r.y1 += clip.height() - r.y2;
        if (r.x1 > 0.0f)
            r.x1 = 0.0f;
        if (r.y1 > 0.0f)
            r.y1 = 0.0f;
        SetWndPos(r.lt);
    }

    Fvector2 CUIGlobalMap::ConvertRealToLocal(const Fvector2& src, bool for_drawing)
    // pixels->pixels (relatively own left-top pos)
    {
        Fvector2 res;
        res.x = (src.x - BoundRect().lt.x) * GetCurrentZoom().x;
        res.y = (src.y - BoundRect().lt.y) * GetCurrentZoom().x;
        return res;
    }

    void CUIGlobalMap::MoveWndDelta(const Fvector2& d)
    {
        inherited::MoveWndDelta(d);
        ClipByVisRect();
        m_mapWnd->UpdateScroll();
    }

    float CUIGlobalMap::CalcOpenRect(const Fvector2& center_point, Frect& map_desired_rect, float tgt_zoom)
    {
        Fvector2 new_center_pt;
        // calculate desired rect in new zoom
        map_desired_rect.set(0.0f, 0.0f, BoundRect().width() * tgt_zoom, BoundRect().height() * tgt_zoom);

        // calculate center point in new zoom (center_point is in identity global map space)
        new_center_pt.set(center_point.x * tgt_zoom, center_point.y * tgt_zoom);
        // get vis width & height
        Frect vis_abs_rect = m_mapWnd->ActiveMapRect();
        float vis_w = vis_abs_rect.width();
        float vis_h = vis_abs_rect.height();
        // calculate center delta from vis rect
        Fvector2 delta_pos;
        delta_pos.set(new_center_pt.x - vis_w * 0.5f, new_center_pt.y - vis_h * 0.5f);

        // correct desired rect
        map_desired_rect.sub(delta_pos.x, delta_pos.y);
        // clamp pos by vis rect
        const Frect& r = map_desired_rect;
        Fvector2 np = r.lt;
        if (r.x2 < vis_w)
            np.x += vis_w - r.x2;
        if (r.y2 < vis_h)
            np.y += vis_h - r.y2;
        if (r.x1 > 0.0f)
            np.x = 0.0f;
        if (r.y1 > 0.0f)
            np.y = 0.0f;
        np.sub(r.lt);
        map_desired_rect.add(np.x, np.y);
        // calculate max way dist
        float dist = 0.f;

        Frect s_rect, t_rect;
        s_rect.div(GetWndRect(), GetCurrentZoom().x, GetCurrentZoom().x);
        t_rect.div(map_desired_rect, tgt_zoom, tgt_zoom);

        Fvector2 cpS, cpT;
        s_rect.getcenter(cpS);
        t_rect.getcenter(cpT);

        dist = cpS.distance_to(cpT);

        return dist;
    }
}