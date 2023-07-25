#pragma once
#include "../monster_state_manager.h"

class CPseudoGigant;

class CStateManagerGigant : public CMonsterStateManager<CPseudoGigant>
{
    typedef CMonsterStateManager<CPseudoGigant> inherited;

public:
    CStateManagerGigant(CPseudoGigant* monster);
    virtual ~CStateManagerGigant();
    virtual void execute();
    virtual void remove_links(CObject* object) { inherited::remove_links(object); }
};

class CPseudoGigantJumper;

class CStateManagerGigantJumper : public CMonsterStateManager<CPseudoGigantJumper>
{
    typedef CMonsterStateManager<CPseudoGigantJumper> inherited;

public:
    CStateManagerGigantJumper(CPseudoGigantJumper* monster);
    virtual ~CStateManagerGigantJumper();

    virtual void execute();
    virtual void remove_links(CObject* object) { inherited::remove_links(object); }
};