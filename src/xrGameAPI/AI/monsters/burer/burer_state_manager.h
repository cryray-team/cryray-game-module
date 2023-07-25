#pragma once
#include "../monster_state_manager.h"

class CBurer;

class CStateManagerBurer : public CMonsterStateManager<CBurer>
{
    typedef CMonsterStateManager<CBurer> inherited;

public:
    CStateManagerBurer(CBurer* monster);
    virtual ~CStateManagerBurer();

    virtual void execute();
    virtual void setup_substates();
    virtual void remove_links(CObject* object) { inherited::remove_links(object); }
};

class CBurer2;

class CStateManagerBurer2 : public CMonsterStateManager<CBurer2>
{
    typedef CMonsterStateManager<CBurer2> inherited;

public:
    CStateManagerBurer2(CBurer2* monster);
    virtual ~CStateManagerBurer2();

    virtual void execute();
    virtual void setup_substates();
    virtual void remove_links(CObject* object) { inherited::remove_links(object); }
};