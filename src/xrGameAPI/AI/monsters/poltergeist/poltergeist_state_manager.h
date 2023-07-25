#pragma once
#include "../monster_state_manager.h"

class CPoltergeist;

class CStateManagerPoltergeist : public CMonsterStateManager<CPoltergeist>
{
    typedef CMonsterStateManager<CPoltergeist> inherited;

public:
    CStateManagerPoltergeist(CPoltergeist* obj);
    virtual ~CStateManagerPoltergeist();

    virtual void reinit();
    virtual void execute();
    virtual void remove_links(CObject* object) { inherited::remove_links(object); }

private:
    u32 time_next_flame_attack;
    u32 time_next_tele_attack;
    u32 time_next_scare_attack;
};

class CPoltergeist2;

class CStateManagerPoltergeist2 : public CMonsterStateManager<CPoltergeist2>
{
    typedef CMonsterStateManager<CPoltergeist2> inherited;

public:
    CStateManagerPoltergeist2(CPoltergeist2* obj);
    virtual ~CStateManagerPoltergeist2();

    virtual void reinit();
    virtual void execute();
    virtual void remove_links(CObject* object) { inherited::remove_links(object); }

private:
    u32 time_next_flame_attack;
    u32 time_next_tele_attack;
    u32 time_next_scare_attack;

    CPolterSpecialAbility2* m_flame;
    CPolterSpecialAbility2* m_tele;
    IC CPolterSpecialAbility2* ability() { return (m_flame ? m_flame : m_tele); }
};