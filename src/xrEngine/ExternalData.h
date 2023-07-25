#pragma once

// OldSerpskiStalker
namespace DATARENDER
{
	class ExteranData
	{
    public:
        ExteranData()
        {
            helmet_with_glass = false;
            outfit_with_glass = false;
            enable_glass = true;
            helmet_condition = 0.0f;
            outfit_condition = 0.0f;
            raindrops_factor = 0.0f;
            raindrops_steps_finished = 0;
        }
	public:
		// Есть ли стекло в головном уборе ?
		bool					helmet_with_glass;

		void					HelmetWithGlassActive(bool read_only)
		{
			helmet_with_glass	= read_only;
		};

		bool					IsHelmetWithGlassActive()
		{ 
			return helmet_with_glass && enable_glass;
		};

		// Есть ли встроенная маска в броне ?
		bool					outfit_with_glass;

		void					OutfitWithGlassActive(bool read_only)
		{
			outfit_with_glass	= read_only;
		};

		bool					IsOutfitWithGlassActive()
		{
			return				outfit_with_glass && enable_glass;
		};

		bool					enable_glass;

		void					GlassActive(bool read_only)
		{
			enable_glass		= read_only;
		};

		bool					IsGlassActive()
		{
			return				enable_glass;
		};

		// Состояние шлема
		float					helmet_condition;

		void					HelmetCondition(float read_only)
		{
			helmet_condition	= read_only;
		};

		float					HelmetConditionActive()
		{
			return				helmet_condition;
		};

		// Состояние брони
		float					outfit_condition;

		void					OutfitCondition(float read_only)
		{
			outfit_condition	= read_only;
		};

		float					OutfitConditionActive()
		{
			return				outfit_condition;
		};

		// Фактор дождя на стекле
        float					raindrops_factor;
        u32						raindrops_steps_finished;
	};
}