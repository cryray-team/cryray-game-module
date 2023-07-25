#pragma once
#include "../monster_state_manager.h"

class CAI_Dog;

class CStateManagerDog : public CMonsterStateManager<CAI_Dog>
{
    typedef CMonsterStateManager<CAI_Dog> inherited;

public:
    CStateManagerDog(CAI_Dog* monster);
    virtual ~CStateManagerDog();

    bool check_eat();
    virtual void execute();
    virtual void remove_links(CObject* object) { inherited::remove_links(object); }
};

class CAI_Dog2;

class CStateManagerDog2 : public CMonsterStateManager<CAI_Dog2>
{
    typedef CMonsterStateManager<CAI_Dog2> inherited;

public:
    CStateManagerDog2(CAI_Dog2* monster);
    virtual ~CStateManagerDog2();

    virtual void execute();
    virtual void remove_links(CObject* object) { inherited::remove_links(object); }
};