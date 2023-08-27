#include "stdafx.h"
#include "WeaponRG6.h"
#include "entity.h"
#include "explosiveRocket.h"
#include "level.h"
#include "MathUtils.h"
#include "actor.h"

#ifdef DEBUG
#include "phdebug.h"
#endif

CWeaponRG6::~CWeaponRG6() {}

BOOL CWeaponRG6::net_Spawn(CSE_Abstract* DC)
{
    BOOL l_res = inheritedSG::net_Spawn(DC);
    if (!l_res)
        return l_res;

    if (m_ammoElapsed.type1 && !getCurrentRocket())
    {
        shared_str grenade_name = m_ammoTypes[0];
        shared_str fake_grenade_name = pSettings->r_string(grenade_name, "fake_grenade_name");

        if (fake_grenade_name.size())
        {
            int k = m_ammoElapsed.type1;
            while (k)
            {
                k--;
                inheritedRL::SpawnRocket(*fake_grenade_name, this);
            }
        }
        //			inheritedRL::SpawnRocket(*fake_grenade_name, this);
    }

    return l_res;
};

void CWeaponRG6::Load(LPCSTR section)
{
    inheritedRL::Load(section);
    inheritedSG::Load(section);
}

#include "inventory.h"
#include "inventoryOwner.h"

void CWeaponRG6::FireStart()
{
    if (GetState() == eIdle && getRocketCount() && m_ammoElapsed.type1)
    {
        inheritedSG::FireStart();

        Fvector p1, d;
        p1.set(get_LastFP());
        d.set(get_LastFD());

        CEntity* E = smart_cast<CEntity*>(H_Parent());
        if (E)
        {
            CInventoryOwner* io = smart_cast<CInventoryOwner*>(H_Parent());
            if (NULL == io->inventory().ActiveItem())
            {
                Msg("current_state: %u", GetState());
                Msg("next_state: %u", GetNextState());
                Msg("item_sect: %s", cNameSect().c_str());
                Msg("H_Parent: %s", H_Parent()->cNameSect().c_str());
            }
            E->g_fireParams(this, p1, d);
        }

        Fmatrix launch_matrix;
        launch_matrix.identity();
        launch_matrix.k.set(d);
        Fvector::generate_orthonormal_basis(launch_matrix.k, launch_matrix.j, launch_matrix.i);
        launch_matrix.c.set(p1);

        if (IsGameTypeSingle() && IsZoomed() && smart_cast<CActor*>(H_Parent()))
        {
            H_Parent()->setEnabled(FALSE);
            setEnabled(FALSE);

            collide::rq_result RQ;
            BOOL HasPick = Level().ObjectSpace.RayPick(p1, d, 300.0f, collide::rqtStatic, RQ, this);

            setEnabled(TRUE);
            H_Parent()->setEnabled(TRUE);

            if (HasPick)
            {
                Fvector Transference;
                Transference.mul(d, RQ.range);
                Fvector res[2];

                u8 canfire0 = TransferenceAndThrowVelToThrowDir(
                    Transference, CRocketLauncher::m_fLaunchSpeed, EffectiveGravity(), res);
                if (canfire0 != 0)
                {
                    d = res[0];
                };
            }
        };

        d.normalize();
        d.mul(m_fLaunchSpeed);
        VERIFY2(_valid(launch_matrix), "CWeaponRG6::FireStart. Invalid launch_matrix");
        CRocketLauncher::LaunchRocket(launch_matrix, d, zero_vel);

        CExplosiveRocket* pGrenade = smart_cast<CExplosiveRocket*>(getCurrentRocket());
        VERIFY(pGrenade);
        pGrenade->SetInitiator(H_Parent()->ID());
        pGrenade->SetRealGrenadeName(m_ammoTypes[m_ammoType.type1]);

        if (OnServer())
        {
            NET_Packet P;
            u_EventGen(P, GE_LAUNCH_ROCKET, ID());
            P.w_u16(u16(getCurrentRocket()->ID()));
            u_EventSend(P);
        }
        dropCurrentRocket();
    }
}

u8 CWeaponRG6::AddCartridge(u8 cnt)
{
    u8 t = inheritedSG::AddCartridge(cnt);
    u8 k = cnt - t;
    shared_str fake_grenade_name = pSettings->r_string(m_ammoTypes[m_ammoType.type1].c_str(), "fake_grenade_name");
    while (k)
    {
        --k;
        inheritedRL::SpawnRocket(*fake_grenade_name, this);
    }
    return t;
}

void CWeaponRG6::OnEvent(NET_Packet& P, u16 type)
{
    inheritedSG::OnEvent(P, type);

    u16 id;
    switch (type)
    {
    case GE_OWNERSHIP_TAKE: {
        P.r_u16(id);
        inheritedRL::AttachRocket(id, this);
    }
    break;
    case GE_OWNERSHIP_REJECT:
    case GE_LAUNCH_ROCKET: {
        bool bLaunch = (type == GE_LAUNCH_ROCKET);
        P.r_u16(id);
        inheritedRL::DetachRocket(id, bLaunch);
    }
    break;
    }
}
