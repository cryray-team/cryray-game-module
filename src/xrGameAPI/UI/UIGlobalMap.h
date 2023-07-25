#pragma once
#include "ui/UIStatic.h"
#include "ui/UIWndCallback.h"
#include "UICustomMap.h"

class CUIMapWnd;
class CUIGlobalMapSpot;

namespace UIGlobalMap
{
    class GAME_API CUIGlobalMap : public UICustomMap::CUICustomMap /*C# Manager*/
    {
        typedef UICustomMap::CUICustomMap inherited;

        shared_str m_prev_active_map;
        CUIMapWnd* m_mapWnd;
        float m_minZoom;
        float m_max_zoom;

    public:
        virtual Fvector2 ConvertRealToLocal(const Fvector2& src, bool for_drawing);
        // pixels->pixels (relatively own left-top pos)

        CUIGlobalMap(CUIMapWnd* pMapWnd);
        virtual ~CUIGlobalMap();

        IC void SetMinZoom(float zoom) { m_minZoom = zoom; }
        IC float GetMinZoom() { return m_minZoom; }
        IC float GetMaxZoom() { return m_max_zoom; }
        IC void SetMaxZoom(float zoom) { m_max_zoom = zoom; }

        virtual bool OnMouseAction(float x, float y, EUIMessages mouse_action);

        CUIMapWnd* MapWnd() { return m_mapWnd; }
        void MoveWndDelta(const Fvector2& d);

        float CalcOpenRect(const Fvector2& center_point, Frect& map_desired_rect, float tgt_zoom);

        void ClipByVisRect();
        virtual void Update();
        void Initialize();

    protected:
        virtual void Init_internal(const shared_str& name, CInifile& pLtx, const shared_str& sect_name, LPCSTR sh_name);
    };
}