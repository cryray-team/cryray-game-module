#pragma once
#include "../action_planner.h"
#include "../property_evaluator_const.h"

#include "../xrGameAPI/UI/UICustomMap.h"
#include "../xrGameAPI/UI/UIGlobalMap.h"
#include "../xrGameAPI/UI/UILevelMap.h"
#include "../xrGameAPI/UI/UIMiniMap.h"

using namespace UICustomMap;
using namespace UIGlobalMap;
using namespace UILevelMap;
using namespace UIMiniMap;

class CUIMapWnd;
class UICustomMap::CUICustomMap;
class CUILevelMap;

class CMapActionPlanner : public CActionPlanner<CUIMapWnd, true>
{
private:
    typedef CActionPlanner<CUIMapWnd, true> inherited;

public:
    CMapActionPlanner();
    virtual ~CMapActionPlanner();
    virtual void setup(CUIMapWnd* object);
    virtual LPCSTR object_name() const;
};
