#pragma once
#include "../control_path_builder.h"

class CPoltergeist;
class CCustomMonster;

namespace POLTERGEIST_MOVEMENT_MANAGER
{
    class CPoltergeisMovementManager : public CControlPathBuilder
    {
        typedef CControlPathBuilder inherited;

        CPoltergeist* m_monster;

    public:
        CPoltergeisMovementManager(CPoltergeist* monster) : inherited((CCustomMonster*)monster), m_monster(monster) {}
        virtual ~CPoltergeisMovementManager() {}

        virtual void move_along_path(CPHMovementControl* movement_control, Fvector& dest_position, float time_delta);

        Fvector CalculateRealPosition();
    };
}

class CPoltergeist2;
class CCustomMonster;

namespace POLTERGEIST2_MOVEMENT_MANAGER
{
    class CPoltergeisMovementManager2 : public CControlPathBuilder
    {
        typedef CControlPathBuilder inherited;

        CPoltergeist2* m_monster;

    public:
        CPoltergeisMovementManager2(CPoltergeist2* monster) : inherited((CCustomMonster*)monster), m_monster(monster) {}
        virtual ~CPoltergeisMovementManager2() {}

        virtual void move_along_path(CPHMovementControl* movement_control, Fvector& dest_position, float time_delta);

        Fvector CalculateRealPosition();
    };
}