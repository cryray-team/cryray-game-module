#pragma once
#include "../monster_state_manager.h"

class CAI_Boar;

class CStateManagerBoar : public CMonsterStateManager<CAI_Boar>
{
    typedef CMonsterStateManager<CAI_Boar> inherited;

public:
    CStateManagerBoar(CAI_Boar* monster);
    virtual ~CStateManagerBoar();

    virtual void execute();
    virtual void remove_links(CObject* object) { inherited::remove_links(object); }
};

class CAI_CBoar2;

class CStateManagerBoar2 : public CMonsterStateManager<CAI_CBoar2>
{
    typedef CMonsterStateManager<CAI_CBoar2> inherited;

public:
    CStateManagerBoar2(CAI_CBoar2* monster);
    virtual ~CStateManagerBoar2();

    virtual void execute();
    virtual void remove_links(CObject* object) { inherited::remove_links(object); }
};