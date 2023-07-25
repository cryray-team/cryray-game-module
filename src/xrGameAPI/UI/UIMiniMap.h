#pragma once
#include "UICustomMap.h"

#include "ui/UIStatic.h"
#include "ui/UIWndCallback.h"

class CUIMapWnd;
class CUIGlobalMapSpot;

namespace UIMiniMap
{
    class GAME_API CUIMiniMap : public UICustomMap::CUICustomMap /*C# Manager*/
    {
        typedef UICustomMap::CUICustomMap inherited;

    public:
        CUIMiniMap();
        virtual ~CUIMiniMap();
        virtual void Draw();
        virtual bool GetPointerTo(const Fvector2& src, float item_radius, Fvector2& pos, float& heading);
        // position and heading for drawing pointer to src pos
        virtual bool NeedShowPointer(Frect r);
        virtual bool IsRectVisible(Frect r);

    protected:
        virtual void UpdateSpots();
        virtual void Init_internal(const shared_str& name, CInifile& pLtx, const shared_str& sect_name, LPCSTR sh_name);
    };
}