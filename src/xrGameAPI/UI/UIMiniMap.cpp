#include "stdafx.h"

#include "level.h"
#include "map_location.h"
#include "map_manager.h"
#include "map_spot.h"
#include "UI/UIMapWnd.h"

#include "UIMiniMap.h"
#include "../../xrEngine/xr_input.h"

namespace UIMiniMap
{
    CUIMiniMap::CUIMiniMap() {}

    CUIMiniMap::~CUIMiniMap() {}

    void CUIMiniMap::Init_internal(const shared_str& name, CInifile& pLtx, const shared_str& sect_name, LPCSTR sh_name)
    {
        inherited::Init_internal(name, pLtx, sect_name, sh_name);
        CUIStatic::SetTextureColor(0x7fffffff);
    }

    void CUIMiniMap::UpdateSpots()
    {
        DetachAll();
        Locations& ls = Level().MapManager().Locations();
        for (Locations_it it = ls.begin(); it != ls.end(); ++it)
            (*it).location->UpdateMiniMap(this);
    }

    void CUIMiniMap::Draw()
    {
        if (CUICustomMap::OldMap())
        {
            inherited::Draw();
            return;
        }

        u32 segments_count = 20;

        xrAPI.UIRender->SetShader(*m_UIStaticItem.GetShader());
        xrAPI.UIRender->StartPrimitive(segments_count * 3, IUIRender::ptTriList, UI().m_currentPointType);

        u32 color = m_UIStaticItem.GetTextureColor();
        float angle = GetHeading();

        float kx = UI().get_current_kx();

        // clip poly
        sPoly2D S;
        S.resize(segments_count);
        float segment_ang = PI_MUL_2 / segments_count;
        float pt_radius = WorkingArea().width() / 2.0f;
        Fvector2 center;
        WorkingArea().getcenter(center);

        float tt_radius = pt_radius / GetWidth();
        float k_tt_height = GetWidth() / GetHeight();

        Fvector2 tt_offset;
        tt_offset.set(m_UIStaticItem.vHeadingPivot);
        tt_offset.x /= GetWidth();
        tt_offset.y /= GetHeight();

        Fvector2 m_scale_;
        m_scale_.set(float(Device.dwWidth) / UI_BASE_WIDTH, float(Device.dwHeight) / UI_BASE_HEIGHT);

        for (u32 idx = 0; idx < segments_count; ++idx)
        {
            float cosPT = _cos(segment_ang * idx + angle);
            float sinPT = _sin(segment_ang * idx + angle);

            float cosTX = _cos(segment_ang * idx);
            float sinTX = _sin(segment_ang * idx);

            S[idx].pt.set(pt_radius * cosPT * kx, -pt_radius * sinPT);
            S[idx].uv.set(tt_radius * cosTX, -tt_radius * sinTX * k_tt_height);
            S[idx].uv.add(tt_offset);
            S[idx].pt.add(center);

            S[idx].pt.x *= m_scale_.x;
            S[idx].pt.y *= m_scale_.y;
        }

        for (u32 idx = 0; idx < segments_count - 2; ++idx)
        {
            xrAPI.UIRender->PushPoint(S[0 + 0].pt.x, S[0 + 0].pt.y, 0, color, S[0 + 0].uv.x, S[0 + 0].uv.y);
            xrAPI.UIRender->PushPoint(S[idx + 2].pt.x, S[idx + 2].pt.y, 0, color, S[idx + 2].uv.x, S[idx + 2].uv.y);
            xrAPI.UIRender->PushPoint(S[idx + 1].pt.x, S[idx + 1].pt.y, 0, color, S[idx + 1].uv.x, S[idx + 1].uv.y);
        }

        xrAPI.UIRender->FlushPrimitive();

        //------------
        CUIWindow::Draw(); // draw childs
    }

    bool CUIMiniMap::GetPointerTo(const Fvector2& src, float item_radius, Fvector2& pos, float& heading)
    {
        if (CUICustomMap::OldMap())
        {
            Frect clip_rect_abs = WorkingArea(); // absolute rect coords
            Frect map_rect_abs;
            GetAbsoluteRect(map_rect_abs);

            Frect rect;
            BOOL res = rect.intersection(clip_rect_abs, map_rect_abs);

            if (!res)
                return false;

            rect = clip_rect_abs;
            rect.sub(map_rect_abs.lt.x, map_rect_abs.lt.y);

            Fbox2 f_clip_rect_local;
            f_clip_rect_local.set(rect.x1, rect.y1, rect.x2, rect.y2);

            Fvector2 f_center;
            f_clip_rect_local.getcenter(f_center);

            Fvector2 f_dir, f_src;

            f_src.set(src.x, src.y);
            f_dir.sub(f_center, f_src);
            f_dir.normalize_safe();
            Fvector2 f_intersect_point;
            res = f_clip_rect_local.Pick2(f_src, f_dir, f_intersect_point);

            if (!res)
                return false;

            heading = -f_dir.getH();

            f_intersect_point.mad(f_intersect_point, f_dir, item_radius);

            pos.set(iFloor(f_intersect_point.x), iFloor(f_intersect_point.y));
        }
        else
        {
            Fvector2 clip_center = GetStaticItem()->GetHeadingPivot();
            float map_radius = WorkingArea().width() / 2.0f;
            Fvector2 direction;

            direction.sub(clip_center, src);
            heading = -direction.getH();

            float kx = UI().get_current_kx();
            float cosPT = _cos(heading);
            float sinPT = _sin(heading);
            pos.set(-map_radius * sinPT * kx, -map_radius * cosPT);
            pos.add(clip_center);
        }

        return true;
    }

    bool CUIMiniMap::NeedShowPointer(Frect r)
    {
        if (CUICustomMap::OldMap())
        {
            Frect map_visible_rect = WorkingArea();
            map_visible_rect.shrink(5, 5);
            Fvector2 pos;
            GetAbsolutePos(pos);
            r.add(pos.x, pos.y);
            return !map_visible_rect.intersected(r);
        }
        else
        {
            Fvector2 clip_center = GetStaticItem()->GetHeadingPivot();

            Fvector2 spot_pos;
            r.getcenter(spot_pos);
            float dist = clip_center.distance_to(spot_pos);
            float spot_radius = r.width() / 2.0f;
            return (dist + spot_radius > WorkingArea().width() / 2.0f);
        }
    }

    bool CUIMiniMap::IsRectVisible(Frect r)
    {
        if (CUICustomMap::OldMap())
        {
            Fvector2 pos;
            GetAbsolutePos(pos);
            r.add(pos.x, pos.y);

            return !!WorkingArea().intersected(r);
        }
        else
        {
            Fvector2 clip_center = GetStaticItem()->GetHeadingPivot();
            float vis_radius = WorkingArea().width() / 2.0f;
            Fvector2 rect_center;
            r.getcenter(rect_center);
            float spot_radius = r.width() / 2.0f;
            return clip_center.distance_to(rect_center) + spot_radius < vis_radius; // assume that all minimap spots are
                                                                                    // circular
        }
    }
}