#include "stdafx.h"
#include "bloodsucker_alien.h"
#include "bloodsucker.h"
#include "bloodsucker2.h"
#include "bloodsucker3.h"
#include "level.h"
#include "Actor.h"
#include "inventory.h"
#include "../xrEngine/CustomHUD.h"

namespace BLOODSUCKER_ALIEN
{
    CAlienEffectorPP::CAlienEffectorPP(const SPPInfo& ppi, EEffectorPPType type) : CEffectorPP(type, flt_max, false)
    {
        state = ppi;
        factor = 0.f;
        target_factor = 1.f;
    }

    CAlienEffectorPP::~CAlienEffectorPP() {}

    BOOL CAlienEffectorPP::Process(SPPInfo& pp)
    {
        inherited::Process(pp);

        if (fsimilar(factor, target_factor))
        {
            target_factor = (target_factor > 0.5f) ? .3f : .6f;
        }

        def_lerp(factor, target_factor, BloodsuckerAlienVampireDetails::PeriodSpeed, Device.fTimeDelta);
        pp.lerp(pp_identity, state, factor);

        return TRUE;
    }

    void CAlienEffectorPP::Destroy()
    {
        fLifeTime = 0.f;
        CAlienEffectorPP* self = this;
        xr_delete(self);
    }

    //////////////////////////////////////////////////////////////////////////
    // Alien Camera Effector
    //////////////////////////////////////////////////////////////////////////

    CAlienEffector::CAlienEffector(ECamEffectorType type, CAI_Bloodsucker* obj) : inherited(type, flt_max)
    {
        dangle_target.set(angle_normalize(Random.randFs(BloodsuckerAlienVampireDetails::DeltaAngleX)),
            angle_normalize(Random.randFs(BloodsuckerAlienVampireDetails::DeltaAngleY)),
            angle_normalize(Random.randFs(BloodsuckerAlienVampireDetails::DeltaAngleZ)));
        dangle_current.set(0.f, 0.f, 0.f);

        monster = obj;

        m_prev_eye_matrix.c = get_head_position(monster);
        m_prev_eye_matrix.k = monster->Direction();
        Fvector::generate_orthonormal_basis(m_prev_eye_matrix.k, m_prev_eye_matrix.j, m_prev_eye_matrix.i);
        m_inertion = 1.f;
        m_current_fov = BloodsuckerAlienVampireDetails::MinFov;
    }

    BOOL CAlienEffector::ProcessCam(SCamEffectorInfo& info)
    {
        // �������������
        Fmatrix Mdef;
        Mdef.identity();
        Mdef.j.set(info.n);
        Mdef.k.set(info.d);
        Mdef.i.crossproduct(info.n, info.d);
        Mdef.c.set(info.p);

        // set angle
        if (angle_lerp(dangle_current.x, dangle_target.x, BloodsuckerAlienVampireDetails::AngleSpeed, Device.fTimeDelta))
        {
            dangle_target.x = angle_normalize(Random.randFs(BloodsuckerAlienVampireDetails::DeltaAngleX));
        }

        if (angle_lerp(dangle_current.y, dangle_target.y, BloodsuckerAlienVampireDetails::AngleSpeed, Device.fTimeDelta))
        {
            dangle_target.y = angle_normalize(Random.randFs(BloodsuckerAlienVampireDetails::DeltaAngleY));
        }

        if (angle_lerp(dangle_current.z, dangle_target.z, BloodsuckerAlienVampireDetails::AngleSpeed, Device.fTimeDelta))
        {
            dangle_target.z = angle_normalize(Random.randFs(BloodsuckerAlienVampireDetails::DeltaAngleZ));
        }

        // update inertion
        Fmatrix cur_matrix;
        cur_matrix.k = monster->Direction();
        cur_matrix.c = get_head_position(monster);

        float rel_dist = m_prev_eye_matrix.c.distance_to(cur_matrix.c) / BloodsuckerAlienVampireDetails::MaxCameraDist;
        clamp(rel_dist, 0.f, 1.f);

        def_lerp(m_inertion, 1 - rel_dist, rel_dist, Device.fTimeDelta);

        // set pos and dir with inertion
        m_prev_eye_matrix.c.inertion(cur_matrix.c, m_inertion);
        m_prev_eye_matrix.k.inertion(cur_matrix.k, m_inertion);
        Fvector::generate_orthonormal_basis_normalized(m_prev_eye_matrix.k, m_prev_eye_matrix.j, m_prev_eye_matrix.i);

        // apply position and direction
        Mdef = m_prev_eye_matrix;

        // set fov
        float rel_speed = monster->m_fCurSpeed / 15.f;
        clamp(rel_speed, 0.f, 1.f);

        float m_target_fov = BloodsuckerAlienVampireDetails::MinFov +
            (BloodsuckerAlienVampireDetails::MaxFov - BloodsuckerAlienVampireDetails::MinFov) * rel_speed;
        def_lerp(m_current_fov, m_target_fov, BloodsuckerAlienVampireDetails::FovSpeed, Device.fTimeDelta);

        info.fFov = m_current_fov;
        //////////////////////////////////////////////////////////////////////////

        // ���������� ���� ��������
        Fmatrix R;
        R.setHPB(dangle_current.x, dangle_current.y, dangle_current.z);

        Fmatrix mR;
        mR.mul(Mdef, R);

        info.d.set(mR.k);
        info.n.set(mR.j);
        info.p.set(mR.c);

        return TRUE;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////
    //
    ///////////////////////////////////////////////////////////////////////////////////////////

    CBloodsuckerAlien::CBloodsuckerAlien() { m_object = 0; }

    CBloodsuckerAlien::~CBloodsuckerAlien() {}

    void CBloodsuckerAlien::init_external(CAI_Bloodsucker* obj) { m_object = obj; }

    void CBloodsuckerAlien::reinit()
    {
        m_active = false;
        m_crosshair_show = false;
    }

    void CBloodsuckerAlien::activate()
    {
        if (m_active)
            return;

        VERIFY(Actor());
        m_object->CControlledActor::install(Actor());
        m_object->CControlledActor::dont_need_turn();

        if (!m_object->EnemyMan.get_enemy())
            m_object->EnemyMan.add_enemy(Actor());

        //.	Actor()->inventory().setSlotsBlocked			(true);
        Actor()->SetWeaponHideState(INV_STATE_BLOCK_ALL, true);

        // hide crosshair
        m_crosshair_show = !!psHUD_Flags.is(HUD_CROSSHAIR_RT);
        if (m_crosshair_show)
            psHUD_Flags.set(HUD_CROSSHAIR_RT, FALSE);

        // Start effector
        m_effector_pp =
            xr_new<BLOODSUCKER_ALIEN::CAlienEffectorPP>(m_object->pp_vampire_effector, EFFECTOR_ID_GEN(EEffectorPPType));
        Actor()->Cameras().AddPPEffector(m_effector_pp);

        m_effector = xr_new<BLOODSUCKER_ALIEN::CAlienEffector>(EFFECTOR_ID_GEN(ECamEffectorType), m_object);
        Actor()->Cameras().AddCamEffector(m_effector);

        // make invisible
        m_object->state_invisible = true;
        m_object->setVisible(false);

        m_active = true;
    }

    void CBloodsuckerAlien::deactivate()
    {
        if (!m_active)
            return;

        m_object->CControlledActor::release();

        Actor()->SetWeaponHideState(INV_STATE_BLOCK_ALL, false);
        if (m_crosshair_show)
            psHUD_Flags.set(HUD_CROSSHAIR_RT, TRUE);

        // Stop camera effector
        Actor()->Cameras().RemoveCamEffector(EFFECTOR_ID_GEN(ECamEffectorType));
        m_effector = 0;

        // Stop postprocess effector
        Actor()->Cameras().RemovePPEffector(EFFECTOR_ID_GEN(EEffectorPPType));
        m_effector_pp->Destroy();
        m_effector_pp = 0;

        m_active = false;

        // make visible
        m_object->state_invisible = false;
        m_object->setVisible(true);
    }
}

namespace BLOODSUCKER2_ALIEN
{
    ////////////////////////////////////////////////////////////////////////////////////
    // CAlienEffectcsPP
    ////////////////////////////////////////////////////////////////////////////////////

    CAlienEffectcsPP::CAlienEffectcsPP(const SPPInfo& ppi, EEffectorPPType type) : CEffectorPP(type, flt_max, false)
    {
        state = ppi;
        factor = 0.f;
        target_factor = 1.f;
    }

    CAlienEffectcsPP::~CAlienEffectcsPP() {}

    BOOL CAlienEffectcsPP::Process(SPPInfo& pp)
    {
        inherited::Process(pp);

        if (fsimilar(factor, target_factor))
        {
            target_factor = (target_factor > 0.5f) ? .3f : .6f;
        }

        def_lerp(factor, target_factor, BloodsuckerAlienVampireDetails::PeriodSpeed, Device.fTimeDelta);
        pp.lerp(pp_identity, state, factor);

        return TRUE;
    }

    void CAlienEffectcsPP::Destroy()
    {
        fLifeTime = 0.f;
        CAlienEffectcsPP* self = this;
        xr_delete(self);
    }

    //////////////////////////////////////////////////////////////////////////
    // Alien Camera Effector
    //////////////////////////////////////////////////////////////////////////


    CAlienEffectcs::CAlienEffectcs(ECamEffectorType type, CAI_Bloodsucker2* obj) : inherited(type, flt_max)
    {
        dangle_target.set(angle_normalize(Random.randFs(BloodsuckerAlienVampireDetails::DeltaAngleX)),
            angle_normalize(Random.randFs(BloodsuckerAlienVampireDetails::DeltaAngleY)),
            angle_normalize(Random.randFs(BloodsuckerAlienVampireDetails::DeltaAngleZ)));
        dangle_current.set(0.f, 0.f, 0.f);

        monster = obj;

        m_prev_eye_matrix.c = get_head_position(monster);
        m_prev_eye_matrix.k = monster->Direction();
        Fvector::generate_orthonormal_basis(m_prev_eye_matrix.k, m_prev_eye_matrix.j, m_prev_eye_matrix.i);
        m_inertion = 1.f;
        m_current_fov = BloodsuckerAlienVampireDetails::MinFov;
    }

    BOOL CAlienEffectcs::ProcessCam(SCamEffectorInfo& info)
    {
        // �������������
        Fmatrix Mdef;
        Mdef.identity();
        Mdef.j.set(info.n);
        Mdef.k.set(info.d);
        Mdef.i.crossproduct(info.n, info.d);
        Mdef.c.set(info.p);

        // set angle
        if (angle_lerp(dangle_current.x, dangle_target.x, BloodsuckerAlienVampireDetails::AngleSpeed, Device.fTimeDelta))
        {
            dangle_target.x = angle_normalize(Random.randFs(BloodsuckerAlienVampireDetails::DeltaAngleX));
        }

        if (angle_lerp(dangle_current.y, dangle_target.y, BloodsuckerAlienVampireDetails::AngleSpeed, Device.fTimeDelta))
        {
            dangle_target.y = angle_normalize(Random.randFs(BloodsuckerAlienVampireDetails::DeltaAngleY));
        }

        if (angle_lerp(dangle_current.z, dangle_target.z, BloodsuckerAlienVampireDetails::AngleSpeed, Device.fTimeDelta))
        {
            dangle_target.z = angle_normalize(Random.randFs(BloodsuckerAlienVampireDetails::DeltaAngleZ));
        }

        // update inertion
        Fmatrix cur_matrix;
        cur_matrix.k = monster->Direction();
        cur_matrix.c = get_head_position(monster);

        float rel_dist = m_prev_eye_matrix.c.distance_to(cur_matrix.c) / BloodsuckerAlienVampireDetails::MaxCameraDist;
        clamp(rel_dist, 0.f, 1.f);

        def_lerp(m_inertion, 1 - rel_dist, rel_dist, Device.fTimeDelta);

        // set pos and dir with inertion
        m_prev_eye_matrix.c.inertion(cur_matrix.c, m_inertion);
        m_prev_eye_matrix.k.inertion(cur_matrix.k, m_inertion);
        Fvector::generate_orthonormal_basis_normalized(m_prev_eye_matrix.k, m_prev_eye_matrix.j, m_prev_eye_matrix.i);

        // apply position and direction
        Mdef = m_prev_eye_matrix;

        // set fov
        float rel_speed = monster->m_fCurSpeed / 15.f;
        clamp(rel_speed, 0.f, 1.f);

        float m_target_fov = BloodsuckerAlienVampireDetails::MinFov +
            (BloodsuckerAlienVampireDetails::MaxFov - BloodsuckerAlienVampireDetails::MinFov) * rel_speed;
        def_lerp(m_current_fov, m_target_fov, BloodsuckerAlienVampireDetails::FovSpeed, Device.fTimeDelta);

        info.fFov = m_current_fov;
        //////////////////////////////////////////////////////////////////////////

        // ���������� ���� ��������
        Fmatrix R;
        R.setHPB(dangle_current.x, dangle_current.y, dangle_current.z);

        Fmatrix mR;
        mR.mul(Mdef, R);

        info.d.set(mR.k);
        info.n.set(mR.j);
        info.p.set(mR.c);

        return TRUE;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////
    //
    ///////////////////////////////////////////////////////////////////////////////////////////

    CBloodsucker2Alien::CBloodsucker2Alien() { m_object = 0; }

    CBloodsucker2Alien::~CBloodsucker2Alien() {}

    void CBloodsucker2Alien::init_external(CAI_Bloodsucker2* obj) { m_object = obj; }

    void CBloodsucker2Alien::reinit()
    {
        m_active = false;
        m_crosshair_show = false;
    }

    void CBloodsucker2Alien::activate()
    {
        if (m_active)
            return;

        VERIFY(Actor());
        m_object->CControlledActor::install(Actor());
        m_object->CControlledActor::dont_need_turn();

        if (!m_object->EnemyMan.get_enemy())
            m_object->EnemyMan.add_enemy(Actor());

        Actor()->SetWeaponHideState(INV_STATE_BLOCK_ALL, true);

        // hide crosshair
        m_crosshair_show = !!psHUD_Flags.is(HUD_CROSSHAIR_RT);
        if (m_crosshair_show)
            psHUD_Flags.set(HUD_CROSSHAIR_RT, FALSE);

        m_effector = xr_new<CAlienEffectcs>(EFFECTOR_ID_GEN(ECamEffectorType), m_object);
        Actor()->Cameras().AddCamEffector(m_effector);

        // make invisible
        m_object->state_invisible = true;
        m_object->setVisible(false);

        m_active = true;
    }

    void CBloodsucker2Alien::deactivate()
    {
        if (!m_active)
            return;

        m_object->CControlledActor::release();

        Actor()->SetWeaponHideState(INV_STATE_BLOCK_ALL, false);
        if (m_crosshair_show)
            psHUD_Flags.set(HUD_CROSSHAIR_RT, TRUE);

        // Stop camera effector
        Actor()->Cameras().RemoveCamEffector(EFFECTOR_ID_GEN(ECamEffectorType));
        m_effector = 0;

        // Stop postprocess effector
        Actor()->Cameras().RemovePPEffector(EFFECTOR_ID_GEN(EEffectorPPType));
        m_effector_pp->Destroy();
        m_effector_pp = 0;

        m_active = false;

        // make visible
        m_object->state_invisible = false;
        m_object->setVisible(true);
    }
}

 namespace BLOODSUCKER3_ALIEN
{
    CAlienEffectsocPP::CAlienEffectsocPP(const SPPInfo& ppi, EEffectorPPType type) : CEffectorPP(type, flt_max, false)
    {
        state = ppi;
        factor = 0.f;
        target_factor = 1.f;
    }

    CAlienEffectsocPP::~CAlienEffectsocPP() {}

    BOOL CAlienEffectsocPP::Process(SPPInfo& pp)
    {
        inherited::Process(pp);

        if (fsimilar(factor, target_factor))
        {
            target_factor = (target_factor > 0.5f) ? .3f : .6f;
        }

        def_lerp(factor, target_factor, BloodsuckerAlienVampireDetails::PeriodSpeed, Device.fTimeDelta);
        pp.lerp(pp_identity, state, factor);

        return TRUE;
    }

    void CAlienEffectsocPP::Destroy()
    {
        fLifeTime = 0.f;
        CAlienEffectsocPP* self = this;
        xr_delete(self);
    }

    CAlienEffectsoc::CAlienEffectsoc(ECamEffectorType type, CAI_Bloodsucker3* obj) : inherited(type, flt_max)
    {
        dangle_target.set(angle_normalize(Random.randFs(BloodsuckerAlienVampireDetails::DeltaAngleX)),
            angle_normalize(Random.randFs(BloodsuckerAlienVampireDetails::DeltaAngleY)),
            angle_normalize(Random.randFs(BloodsuckerAlienVampireDetails::DeltaAngleZ)));
        dangle_current.set(0.f, 0.f, 0.f);

        monster = obj;

        m_prev_eye_matrix.c = get_head_position(monster);
        m_prev_eye_matrix.k = monster->Direction();
        Fvector::generate_orthonormal_basis(m_prev_eye_matrix.k, m_prev_eye_matrix.j, m_prev_eye_matrix.i);
        m_inertion = 1.f;
        m_current_fov = BloodsuckerAlienVampireDetails::MinFov;
    }

    BOOL CAlienEffectsoc::ProcessCam(SCamEffectorInfo& info)
    {
        // �������������
        Fmatrix Mdef;
        Mdef.identity();
        Mdef.j.set(info.n);
        Mdef.k.set(info.d);
        Mdef.i.crossproduct(info.n, info.d);
        Mdef.c.set(info.p);

        // set angle
        if (angle_lerp(dangle_current.x, dangle_target.x, BloodsuckerAlienVampireDetails::AngleSpeed, Device.fTimeDelta))
        {
            dangle_target.x = angle_normalize(Random.randFs(BloodsuckerAlienVampireDetails::DeltaAngleX));
        }

        if (angle_lerp(dangle_current.y, dangle_target.y, BloodsuckerAlienVampireDetails::AngleSpeed, Device.fTimeDelta))
        {
            dangle_target.y = angle_normalize(Random.randFs(BloodsuckerAlienVampireDetails::DeltaAngleY));
        }

        if (angle_lerp(dangle_current.z, dangle_target.z, BloodsuckerAlienVampireDetails::AngleSpeed, Device.fTimeDelta))
        {
            dangle_target.z = angle_normalize(Random.randFs(BloodsuckerAlienVampireDetails::DeltaAngleZ));
        }

        // update inertion
        Fmatrix cur_matrix;
        cur_matrix.k = monster->Direction();
        cur_matrix.c = get_head_position(monster);

        float rel_dist = m_prev_eye_matrix.c.distance_to(cur_matrix.c) / BloodsuckerAlienVampireDetails::MaxCameraDist;
        clamp(rel_dist, 0.f, 1.f);

        def_lerp(m_inertion, 1 - rel_dist, rel_dist, Device.fTimeDelta);

        // set pos and dir with inertion
        m_prev_eye_matrix.c.inertion(cur_matrix.c, m_inertion);
        m_prev_eye_matrix.k.inertion(cur_matrix.k, m_inertion);
        Fvector::generate_orthonormal_basis_normalized(m_prev_eye_matrix.k, m_prev_eye_matrix.j, m_prev_eye_matrix.i);

        // apply position and direction
        Mdef = m_prev_eye_matrix;

        // set fov
        float rel_speed = monster->m_fCurSpeed / 15.f;
        clamp(rel_speed, 0.f, 1.f);

        float m_target_fov = BloodsuckerAlienVampireDetails::MinFov +
            (BloodsuckerAlienVampireDetails::MaxFov - BloodsuckerAlienVampireDetails::MinFov) * rel_speed;
        def_lerp(m_current_fov, m_target_fov, BloodsuckerAlienVampireDetails::FovSpeed, Device.fTimeDelta);

        info.fFov = m_current_fov;
        //////////////////////////////////////////////////////////////////////////

        // ���������� ���� ��������
        Fmatrix R;
        R.setHPB(dangle_current.x, dangle_current.y, dangle_current.z);

        Fmatrix mR;
        mR.mul(Mdef, R);

        info.d.set(mR.k);
        info.n.set(mR.j);
        info.p.set(mR.c);

        return TRUE;
    }

    CBloodsucker3Alien::CBloodsucker3Alien() { m_object = 0; }

    CBloodsucker3Alien::~CBloodsucker3Alien() {}

    void CBloodsucker3Alien::init_external(CAI_Bloodsucker3* obj) { m_object = obj; }

    void CBloodsucker3Alien::reinit()
    {
        m_active = false;
        m_crosshair_show = false;
    }

    void CBloodsucker3Alien::activate()
    {
        if (m_active)
            return;

        VERIFY(Actor());
        m_object->CControlledActor::install(Actor());
        m_object->CControlledActor::dont_need_turn();

        if (!m_object->EnemyMan.get_enemy())
            m_object->EnemyMan.add_enemy(Actor());

        Actor()->SetWeaponHideState(INV_STATE_BLOCK_ALL, true);

        // hide crosshair
        m_crosshair_show = !!psHUD_Flags.is(HUD_CROSSHAIR_RT);
        if (m_crosshair_show)
            psHUD_Flags.set(HUD_CROSSHAIR_RT, FALSE);

        // Start effector
        m_effector_pp = xr_new<CAlienEffectsocPP>(m_object->pp_vampire_effector, EFFECTOR_ID_GEN(EEffectorPPType));
        Actor()->Cameras().AddPPEffector(m_effector_pp);

        m_effector = xr_new<CAlienEffectsoc>(EFFECTOR_ID_GEN(ECamEffectorType), m_object);
        Actor()->Cameras().AddCamEffector(m_effector);

        // make invisible
        m_object->state_invisible = true;
        m_object->setVisible(false);

        m_active = true;
    }

    void CBloodsucker3Alien::deactivate()
    {
        if (!m_active)
            return;

        m_object->CControlledActor::release();

        Actor()->SetWeaponHideState(INV_STATE_BLOCK_ALL, false);
        if (m_crosshair_show)
            psHUD_Flags.set(HUD_CROSSHAIR_RT, TRUE);

        // Stop camera effector
        Actor()->Cameras().RemoveCamEffector(EFFECTOR_ID_GEN(ECamEffectorType));
        m_effector = 0;

        // Stop postprocess effector
        Actor()->Cameras().RemovePPEffector(EFFECTOR_ID_GEN(EEffectorPPType));
        m_effector_pp->Destroy();
        m_effector_pp = 0;

        m_active = false;

        // make visible
        m_object->state_invisible = false;
        m_object->setVisible(true);
    }
}