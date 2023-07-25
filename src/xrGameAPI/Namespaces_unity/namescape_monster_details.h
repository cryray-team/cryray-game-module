#pragma once

namespace bloodsuckerDetails
{
	namespace bloodsucker
	{
		// CoP Class
		constexpr float default_critical_hit_chance = 0.25f;
		constexpr float default_hit_camera_effector_angle = 0;
		constexpr float default_critical_hit_camera_effector_angle = 3.1415f / 6;

		constexpr float default_camera_effector_move_angular_speed = 1.5f;
		constexpr u32 default_visibility_state_change_min_delay = 1000;

		constexpr float default_full_visibility_radius = 5;
		constexpr float default_partial_visibility_radius = 10;
		constexpr float default_runaway_invisible_time = 3000;

		constexpr const char* full_visibility_radius_string = "full_visibility_radius";
        constexpr const char* partial_visibility_radius_string = "partial_visibility_radius";
        constexpr const char* visibility_state_change_min_delay_string = "visibility_state_change_min_delay";

		// CS Class
        constexpr u32 default_invisibility_activate_delay = 4000;
	} // namespace bloodsucker
} // namespace bloodsuckerDetails

namespace bloodsuckerAttackState
{
	namespace bloodsucker
	{
		constexpr u32 encircle_time = 3000;
		constexpr float loose_health_diff = 0.15f;

		constexpr u32 change_behaviour_time = 1000;
		constexpr float invisibility_dist2enemy = 5.f;
	} // namespace bloodsucker
} // namespace bloodsuckerAttackState

namespace BloodsuckerAlienVampireDetails
{
	constexpr float DeltaAngleX = 10 * PI / 180;
	constexpr float DeltaAngleY = 10.0f * PI / 180.0f;
	constexpr float DeltaAngleZ = 10.0f * PI / 180.0f;
	constexpr float AngleSpeed = 0.2f;

	constexpr float Best_distance = 0.3f;
	constexpr float Time_attack = 0.2f;

	constexpr int periods = 2;

	constexpr float MinFov = 70.0f;
	constexpr float MaxFov = 175.0f;
	constexpr float FovSpeed = 80.0f;
	constexpr float MaxCameraDist = 3.5f;

	constexpr float PeriodSpeed = 0.3f;
} // namespace BloodsuckerAlienVampireDetails

#define RAD_TO_PERC(rad) ((rad - PI_DIV_2) / (BloodsuckerAlienVampireDetails::periods * PI_MUL_2))
#define PERC_TO_RAD(perc) (perc * (BloodsuckerAlienVampireDetails::periods * PI_MUL_2) + PI_DIV_2)

#define EFFECTOR_ID_GEN(type) (type(u32(u64(this) & u32(-1))))

namespace controllerDetails
{
	namespace controller
	{
		// default settings for tube fire:
		constexpr float default_tube_condition_see_duration = 1000;
		constexpr float default_tube_condition_min_delay = 10000;
		constexpr float default_tube_condition_min_distance = 10;
		constexpr float default_stamina_hit = 0.2f;
	} // namespace controller
} // namespace controllerDetails

namespace DogStateManagerDetails
{
	namespace dog
	{
		constexpr float atack_decision_maxdist = 6.f;
	}
} // namespace DogStateManagerDetails

namespace MonsterAuraDetails
{
	static const char* const s_pp_effector_name_string = "_pp_effector_name";
	static const char* const s_pp_highest_at_string = "_pp_highest_at";
	static const char* const s_linear_factor_string = "_linear_factor";
	static const char* const s_quadratic_factor_string = "_quadratic_factor";
	static const char* const s_max_power_string = "_max_power";
	static const char* const s_max_distance_string = "_max_distance";
	static const char* const s_sound_string = "_sound";
	static const char* const s_detect_sound_string = "_detect_sound";
	static const char* const s_enable_for_dead_string = "_enable_for_dead";
} // namespace MonsterAuraDetails
