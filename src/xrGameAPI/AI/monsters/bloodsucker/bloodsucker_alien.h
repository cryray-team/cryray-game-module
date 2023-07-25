#pragma once

#include "../xrGameAPI/Actor/ActorEffector.h"
#include "HudItem.h"

class CAI_Bloodsucker;
class CAI_Bloodsucker2;
class CAI_Bloodsucker3;

namespace BLOODSUCKER_ALIEN
{
	class CAlienEffector : public CEffectorCam
    {
        typedef CEffectorCam inherited;

        float m_time_total;
        Fvector dangle_target;
        Fvector dangle_current;

        CAI_Bloodsucker* monster;

        float m_current_fov;
        Fmatrix m_prev_eye_matrix;
        float m_inertion;

    public:
        CAlienEffector(ECamEffectorType type, CAI_Bloodsucker* obj);
        virtual BOOL ProcessCam(SCamEffectorInfo& info);
    };
	
    class CAlienEffectorPP : public CEffectorPP
    {
        typedef CEffectorPP inherited;

        SPPInfo state;
        float factor;
        float target_factor;

    public:
        CAlienEffectorPP(const SPPInfo& ppi, EEffectorPPType type);
        virtual ~CAlienEffectorPP();

        void Update(float new_factor) { factor = new_factor; }
        void Destroy();

    private:
        virtual BOOL Process(SPPInfo& pp);
    };
	
    class CBloodsuckerAlien
    {
        CAI_Bloodsucker* m_object;

        bool m_active;

        CAlienEffector* m_effector;
        CAlienEffectorPP* m_effector_pp;

        bool m_crosshair_show;

    public:
        CBloodsuckerAlien();
        ~CBloodsuckerAlien();

        void init_external(CAI_Bloodsucker* obj);
        void reinit();

        void activate();
        void deactivate();

        bool active() { return m_active; }
    };
}

namespace BLOODSUCKER2_ALIEN
{
    class CAlienEffectcs : public CEffectorCam
    {
        typedef CEffectorCam inherited;

        float m_time_total;
        Fvector dangle_target;
        Fvector dangle_current;

        CAI_Bloodsucker2* monster;

        float m_current_fov;
        Fmatrix m_prev_eye_matrix;
        float m_inertion;

    public:
        CAlienEffectcs(ECamEffectorType type, CAI_Bloodsucker2* obj);
        virtual BOOL ProcessCam(SCamEffectorInfo& info);
    };

    class CAlienEffectcsPP : public CEffectorPP
    {
        typedef CEffectorPP inherited;

        SPPInfo state;
        float factor;
        float target_factor;

    public:
        CAlienEffectcsPP(const SPPInfo& ppi, EEffectorPPType type);
        virtual ~CAlienEffectcsPP();

        void Update(float new_factor) { factor = new_factor; }
        void Destroy();

    private:
        virtual BOOL Process(SPPInfo& pp);
    };

    class CBloodsucker2Alien
    {
        CAI_Bloodsucker2* m_object;

        bool m_active;

        CAlienEffectcs* m_effector;
        CAlienEffectcsPP* m_effector_pp;

        bool m_crosshair_show;

    public:
        CBloodsucker2Alien();
        ~CBloodsucker2Alien();

        void init_external(CAI_Bloodsucker2* obj);
        void reinit();

        void activate();
        void deactivate();

        bool active() { return m_active; }
    };
}

namespace BLOODSUCKER3_ALIEN
{
    class CAlienEffectsoc : public CEffectorCam
    {
        typedef CEffectorCam inherited;

        float m_time_total;
        Fvector dangle_target;
        Fvector dangle_current;

        CAI_Bloodsucker3* monster;

        float m_current_fov;
        Fmatrix m_prev_eye_matrix;
        float m_inertion;

    public:
        CAlienEffectsoc(ECamEffectorType type, CAI_Bloodsucker3* obj);
        virtual BOOL ProcessCam(SCamEffectorInfo& info);
    };

    class CAlienEffectsocPP : public CEffectorPP
    {
        typedef CEffectorPP inherited;

        SPPInfo state;
        float factor;
        float target_factor;

    public:
        CAlienEffectsocPP(const SPPInfo& ppi, EEffectorPPType type);
        virtual ~CAlienEffectsocPP();

        void Update(float new_factor) { factor = new_factor; }
        void Destroy();

    private:
        virtual BOOL Process(SPPInfo& pp);
    };

    class CBloodsucker3Alien
    {
        CAI_Bloodsucker3* m_object;

        bool m_active;

        CAlienEffectsoc* m_effector;
        CAlienEffectsocPP* m_effector_pp;

        bool m_crosshair_show;

    public:
        CBloodsucker3Alien();
        ~CBloodsucker3Alien();

        void init_external(CAI_Bloodsucker3* obj);
        void reinit();

        void activate();
        void deactivate();

        bool active() { return m_active; }
    };
}