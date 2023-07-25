#pragma once
#include "../monster_state_manager.h"

class CAI_Bloodsucker;
class CAI_Bloodsucker2;
class CAI_Bloodsucker3;

class CStateManagerBloodsucker : public CMonsterStateManager<CAI_Bloodsucker>
{
    typedef CMonsterStateManager<CAI_Bloodsucker> inherited;

public:
    CStateManagerBloodsucker(CAI_Bloodsucker* monster);
    virtual ~CStateManagerBloodsucker();

    virtual void execute();
    virtual void update();
    void drag_object();
    virtual void remove_links(CObject* object) { inherited::remove_links(object); }
    bool check_vampire();
};

class CStateManagerBloodsucker2 : public CMonsterStateManager<CAI_Bloodsucker2>
{
    typedef CMonsterStateManager<CAI_Bloodsucker2> inherited;

public:
    CStateManagerBloodsucker2(CAI_Bloodsucker2* monster);
    virtual ~CStateManagerBloodsucker2();

    virtual void execute();
    virtual void update();
    void drag_object();
    virtual void remove_links(CObject* object) { inherited::remove_links(object); }
};

class CStateManagerBloodsucker3 : public CMonsterStateManager<CAI_Bloodsucker3>
{
    typedef CMonsterStateManager<CAI_Bloodsucker3> inherited;

public:
    CStateManagerBloodsucker3(CAI_Bloodsucker3* monster);
    virtual ~CStateManagerBloodsucker3();

    virtual void execute();
    virtual void remove_links(CObject* object) { inherited::remove_links(object); }
    bool check_vampire();
};