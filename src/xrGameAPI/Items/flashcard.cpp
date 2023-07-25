#include "StdAfx.h"
#include "Flashcard.h"

#include "Actor.h"
#include "script_engine.h"
#include "ai_space.h"

CFlashCard::CFlashCard() {}

CFlashCard::~CFlashCard() {}

BOOL CFlashCard::net_Spawn(CSE_Abstract* DC) { return (inherited::net_Spawn(DC)); }

void CFlashCard::Load(LPCSTR section) { inherited::Load(section); }

void CFlashCard::net_Destroy() { inherited::net_Destroy(); }

void CFlashCard::shedule_Update(u32 dt) { inherited::shedule_Update(dt); }

void CFlashCard::UpdateCL() { inherited::UpdateCL(); }

void CFlashCard::OnH_A_Chield() { inherited::OnH_A_Chield(); }

void CFlashCard::OnH_B_Independent(bool just_before_destroy) { inherited::OnH_B_Independent(just_before_destroy); }

void CFlashCard::renderable_Render() { inherited::renderable_Render(); }

bool CFlashCard::UseBy(CEntityAlive* entity_alive)
{
    CInventoryOwner* IO = dynamic_cast<CInventoryOwner*>(entity_alive);
    CActor* actor = NULL;
    R_ASSERT(IO);
    actor = dynamic_cast<CActor*>(IO);
    R_ASSERT(actor);

    LPCSTR flash_use{};
    luabind::functor<LPCSTR> m_functor_flash;
    R_ASSERT(ai().script_engine().functor("_g_cryray.flash_info_use", m_functor_flash));
    flash_use = m_functor_flash();

    return inherited::UseBy(entity_alive);
}

bool CFlashCard::Empty() const { return inherited::Empty(); }