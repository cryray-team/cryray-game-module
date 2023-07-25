#pragma once
#include "UICustomMap.h"
#include "UILevelMap.h"
#include "UIGlobalMap.h"

#include "ui/UIStatic.h"
#include "ui/UIWndCallback.h"

class CUIMapWnd;
class CUIGlobalMapSpot;

namespace UILevelMap
{
    class GAME_API CUILevelMap : public UICustomMap::CUICustomMap /*C# Manager*/
    {
        typedef UICustomMap::CUICustomMap inherited;

        CUIMapWnd* m_mapWnd;
        Frect m_GlobalRect; // virtual map size (meters)
        CUILevelMap(const CUILevelMap& obj) {}

    public:
        CUILevelMap(CUIMapWnd*);
        virtual ~CUILevelMap();
        const Frect& GlobalRect() const { return m_GlobalRect; }
        void SetGlobalRect(const Frect& r) { m_GlobalRect = r; }
        virtual void Draw();
        virtual void Show(bool status);
        virtual void Update();
        virtual bool OnMouseAction(float x, float y, EUIMessages mouse_action);
        virtual void SendMessage(CUIWindow* pWnd, s16 msg, void* pData);

        Frect CalcWndRectOnGlobal();
        CUIMapWnd* MapWnd() { return m_mapWnd; }

        virtual void OnFocusLost();

    protected:
        virtual void UpdateSpots();
        virtual void Init_internal(const shared_str& name, CInifile& pLtx, const shared_str& sect_name, LPCSTR sh_name);
    };
}