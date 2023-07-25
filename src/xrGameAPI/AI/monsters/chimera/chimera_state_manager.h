#pragma once
#include "../monster_state_manager.h"

class CChimera;

class CStateManagerChimera : public CMonsterStateManager<CChimera>
{
private:
    typedef CMonsterStateManager<CChimera> inherited;

public:
    CStateManagerChimera(CChimera* obj);
    virtual ~CStateManagerChimera();

    virtual void execute();
    virtual void remove_links(CObject* object) { inherited::remove_links(object); }
};

class CChimera2;

class CStateManagerChimera2 : public CMonsterStateManager<CChimera2>
{
    typedef CMonsterStateManager<CChimera2> inherited;

public:
    CStateManagerChimera2(CChimera2* obj);
    virtual ~CStateManagerChimera2();

    virtual void execute();
    virtual void remove_links(CObject* object) { inherited::remove_links(object); }
};