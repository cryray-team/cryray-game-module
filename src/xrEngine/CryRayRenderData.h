#pragma once

namespace CryRayRenderData
{
    class ENGINE_API ExternalData
    {
    public:
        ExternalData();
        ~ExternalData();

        bool helmet_with_glass;

        void HelmetWithGlassActive(bool read_only);

        bool IsHelmetWithGlassActive();

        bool outfit_with_glass;

        void OutfitWithGlassActive(bool read_only);

        bool IsOutfitWithGlassActive();

        bool enable_glass;

        void GlassActive(bool read_only);

        bool IsGlassActive();

        float helmet_condition;

        void HelmetCondition(float read_only);

        float HelmetConditionActive();

        float outfit_condition;

        void OutfitCondition(float read_only);

        float OutfitConditionActive();

        float raindrops_factor;

        u32 raindrops_steps_finished;

        Fmatrix m_script_params;

        Fvector4 hud_params;
        Fvector4 m_blender_mode;

        int hit_power_factor;
        float hit_power_power;
    };
}

extern ENGINE_API CryRayRenderData::ExternalData* pCRRenderData;