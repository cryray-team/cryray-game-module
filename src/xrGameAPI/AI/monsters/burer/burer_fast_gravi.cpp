#include "stdafx.h"
#include "burer_fast_gravi.h"
#include "burer.h"
#include "../control_animation_base.h"
#include "../control_direction_base.h"
#include "../control_movement_base.h"

namespace BURER_FAST_GRAVI
{
    bool CBurerFastGravi::check_start_conditions()
    {
        if (is_active())
            return false;
        if (m_man->is_captured_pure())
            return false;
        if (!m_object->EnemyMan.get_enemy())
            return false;

        return true;
    }

    void CBurerFastGravi::activate()
    {
        m_man->subscribe(this, ControlCom::eventTAChange);
        m_object->dir().face_target(m_object->EnemyMan.get_enemy());
    }

    void CBurerFastGravi::deactivate() { m_man->unsubscribe(this, ControlCom::eventTAChange); }

    void CBurerFastGravi::on_event(ControlCom::EEventType type, ControlCom::IEventData* data)
    {
        if (type == ControlCom::eventTAChange)
        {
            STripleAnimEventData* event_data = (STripleAnimEventData*)data;
            if (event_data->m_current_state == eStateExecute)
            {
                process_hit();
                m_object->com_man().ta_pointbreak();
                m_man->deactivate(this);
            }
        }
    }

    void CBurerFastGravi::process_hit()
    {
        m_object->HitEntity(m_object->EnemyMan.get_enemy(), 1.f, 100.f, m_object->Direction());
    }
}

namespace BURER2_FAST_GRAVI
{
    bool CBurer2FastGravi::check_start_conditions()
    {
        if (is_active())
            return false;
        if (m_man->is_captured_pure())
            return false;
        if (!m_object->EnemyMan.get_enemy())
            return false;

        return true;
    }

    void CBurer2FastGravi::activate()
    {
        CBurer2* Burer2 = smart_cast<CBurer2*>(m_object);
        m_man->subscribe(this, ControlCom::eventTAChange);
        m_object->com_man().ta_activate(Burer2->anim_triple_gravi);

        m_object->dir().face_target(m_object->EnemyMan.get_enemy());
    }

    void CBurer2FastGravi::deactivate() { m_man->unsubscribe(this, ControlCom::eventTAChange); }

    void CBurer2FastGravi::on_event(ControlCom::EEventType type, ControlCom::IEventData* data)
    {
        if (type == ControlCom::eventTAChange)
        {
            STripleAnimEventData* event_data = (STripleAnimEventData*)data;
            if (event_data->m_current_state == eStateExecute)
            {
                process_hit();
                m_object->com_man().ta_pointbreak();
                m_man->deactivate(this);
            }
        }
    }

    void CBurer2FastGravi::process_hit()
    {
        m_object->HitEntity(m_object->EnemyMan.get_enemy(), 1.f, 100.f, m_object->Direction());
    }
}