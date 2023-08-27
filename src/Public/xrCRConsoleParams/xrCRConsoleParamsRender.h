#pragma once

#include "../xrCore/xrCore.h"
#include "../xrCore/PPMdType.h"

#ifdef XRCRCONSOLE_PARAMS_EXPORTS
#define XRCRCONSOLE_PARAMS_API __declspec(dllexport)
#else
#define XRCRCONSOLE_PARAMS_API __declspec(dllimport)
#endif

enum
{
    VOLUMETRIC_SUNSHAFTS = 0,
    OGSE_SUNSHAFTS,
    MANOWAR_SUNSHAFTS,
    VOLUMETRIC_OGSE_SUNSHAFTS,
    VOLUMETRIC_MANOWAR_SUNSHAFTS,
};

enum
{
    RS_CLEAR_MODELS = (1 << 0),
    RS_USE_PRECOMPILED_SHADERS = (1 << 1),
    RS_NO_SCALE_OF_FADE_GRASS = (1 << 2),
    /**/
    /**/
    /**/
    /**/
    /**/
    RS_FLAG_TERRAIN_PREPASS = (1 << 8),
    /**/
    /**/
    //RS_DETAIL_BUMP = (1 << 11),
    //RS_STEEP_PARALLAX = (1 << 12),
};

enum
{
    RS_TIME_SINCE_LAUNCH = (1 << 0),
    RS_FPS_INFO = (1 << 1),
    /**/
    RS_FLAG_DOF = (1 << 3),
    RS_OPT_SHADOW_GEOM = (1 << 4),
    RS_FLAG_DOF_WEATHER = (1 << 5),
    RS_GLOSS_BUILD_2218 = (1 << 6),
    RS_SUN_FLARES = (1 << 7),
    RS_ACTOR_SHADOW = (1 << 8),
    RS_GRASS_SHADOW = (1 << 9),
    RS_FLAG_SHADER_HIT = (1 << 10),
    RS_FLAG_STATIC_DROPS = (1 << 11),
    RS_FLAG_SHADER_STAMINA = (1 << 12),
};

extern XRCRCONSOLE_PARAMS_API int debug_ao;
extern XRCRCONSOLE_PARAMS_API int ps_r2_qsync;
extern XRCRCONSOLE_PARAMS_API int opt_static;
extern XRCRCONSOLE_PARAMS_API int opt_dynamic;
extern XRCRCONSOLE_PARAMS_API int ps_render_volumetric_fog;
extern XRCRCONSOLE_PARAMS_API int ps_r__tf_Anisotropic;

extern XRCRCONSOLE_PARAMS_API u32 ps_r_type_aa;
extern XRCRCONSOLE_PARAMS_API u32 ps_r_bloom_type;
extern XRCRCONSOLE_PARAMS_API u32 ps_r_sunshafts_mode;

extern XRCRCONSOLE_PARAMS_API float ps_r_gpu_wait_time;

extern XRCRCONSOLE_PARAMS_API float ps_r2_sun_shafts_min;
extern XRCRCONSOLE_PARAMS_API float ps_r2_sun_shafts_value;

extern XRCRCONSOLE_PARAMS_API float ps_r2_ls_squality;
extern XRCRCONSOLE_PARAMS_API float ps_r2_slight_fade;

extern XRCRCONSOLE_PARAMS_API float debug_fog_height;
extern XRCRCONSOLE_PARAMS_API float debug_fog_density;
extern XRCRCONSOLE_PARAMS_API float debug_fog_max_dist;
extern XRCRCONSOLE_PARAMS_API float debug_fog_min_dist;

extern XRCRCONSOLE_PARAMS_API float ps_r2_tnmp_a;
extern XRCRCONSOLE_PARAMS_API float ps_r2_tnmp_b;
extern XRCRCONSOLE_PARAMS_API float ps_r2_tnmp_c;
extern XRCRCONSOLE_PARAMS_API float ps_r2_tnmp_d;
extern XRCRCONSOLE_PARAMS_API float ps_r2_tnmp_e;
extern XRCRCONSOLE_PARAMS_API float ps_r2_tnmp_f;
extern XRCRCONSOLE_PARAMS_API float ps_r2_tnmp_w;
extern XRCRCONSOLE_PARAMS_API float ps_r2_tnmp_exposure;
extern XRCRCONSOLE_PARAMS_API float ps_r2_tnmp_gamma;
extern XRCRCONSOLE_PARAMS_API float ps_r2_tnmp_onoff;

extern XRCRCONSOLE_PARAMS_API float ps_r_ss_sunshafts_length;
extern XRCRCONSOLE_PARAMS_API float ps_r_ss_sunshafts_radius;
extern XRCRCONSOLE_PARAMS_API float ps_r_prop_ss_radius;
extern XRCRCONSOLE_PARAMS_API float ps_r_prop_ss_blend;
extern XRCRCONSOLE_PARAMS_API float ps_r_prop_ss_sample_step_phase0;
extern XRCRCONSOLE_PARAMS_API float ps_r_prop_ss_sample_step_phase1;

extern XRCRCONSOLE_PARAMS_API float ps_r2_gloss_min;
extern XRCRCONSOLE_PARAMS_API float ps_r2_gloss_factor;

extern XRCRCONSOLE_PARAMS_API float ps_r2_img_exposure;
extern XRCRCONSOLE_PARAMS_API float ps_r2_img_gamma;
extern XRCRCONSOLE_PARAMS_API float ps_r2_img_saturation;

extern XRCRCONSOLE_PARAMS_API float ps_r__LOD;
extern XRCRCONSOLE_PARAMS_API float ps_r__tf_Mipbias;

extern XRCRCONSOLE_PARAMS_API float OLES_SUN_LIMIT_27_01_07;
extern XRCRCONSOLE_PARAMS_API float r_color_correction;

extern XRCRCONSOLE_PARAMS_API float ps_volumetric_intensity;
extern XRCRCONSOLE_PARAMS_API float ps_volumetric_distance;
extern XRCRCONSOLE_PARAMS_API float ps_volumetric_quality;

extern XRCRCONSOLE_PARAMS_API float ps_ssfx_wpn_dof_2;

extern XRCRCONSOLE_PARAMS_API float ps_dof_sky_weather;
extern XRCRCONSOLE_PARAMS_API float ps_dof_kernel_size_weather;

extern XRCRCONSOLE_PARAMS_API float r_dtex_range;

extern XRCRCONSOLE_PARAMS_API float font_size_x;
extern XRCRCONSOLE_PARAMS_API float font_size_y;

extern XRCRCONSOLE_PARAMS_API Fvector ps_r2_img_cg;
extern XRCRCONSOLE_PARAMS_API Fvector cryray_test_params;
extern XRCRCONSOLE_PARAMS_API Fvector ps_r2_drops_control;

extern XRCRCONSOLE_PARAMS_API Fvector3 ps_dof_weather;
extern XRCRCONSOLE_PARAMS_API Fvector3 ps_ssfx_wetness_multiplier;
extern XRCRCONSOLE_PARAMS_API Fvector3 ps_ssfx_shadow_cascades;

extern XRCRCONSOLE_PARAMS_API Fvector4 ps_ssfx_wpn_dof_1;
extern XRCRCONSOLE_PARAMS_API Fvector3 ps_ssfx_shadow_cascades;

extern XRCRCONSOLE_PARAMS_API Fvector4 ps_r2_mask_control;
extern XRCRCONSOLE_PARAMS_API Fvector4 ps_pp_bloom_thresh;
extern XRCRCONSOLE_PARAMS_API Fvector4 ps_pp_bloom_weight;

extern XRCRCONSOLE_PARAMS_API Fvector4 ps_dev_param_1;
extern XRCRCONSOLE_PARAMS_API Fvector4 ps_dev_param_2;
extern XRCRCONSOLE_PARAMS_API Fvector4 ps_dev_param_3;
extern XRCRCONSOLE_PARAMS_API Fvector4 ps_dev_param_4;
extern XRCRCONSOLE_PARAMS_API Fvector4 ps_dev_param_5;
extern XRCRCONSOLE_PARAMS_API Fvector4 ps_dev_param_6;
extern XRCRCONSOLE_PARAMS_API Fvector4 ps_dev_param_7;
extern XRCRCONSOLE_PARAMS_API Fvector4 ps_dev_param_8;

extern XRCRCONSOLE_PARAMS_API Fvector4 ps_pp_bloom_thresh;
extern XRCRCONSOLE_PARAMS_API Fvector4 ps_pp_bloom_weight;

extern XRCRCONSOLE_PARAMS_API xr_token sunshafts_mode_token[];
extern XRCRCONSOLE_PARAMS_API xr_token type_aa_token[];
extern XRCRCONSOLE_PARAMS_API xr_token qbloom_type_token[];

extern XRCRCONSOLE_PARAMS_API Flags32 CryRayFlags32Render;
extern XRCRCONSOLE_PARAMS_API Flags32 CryRayFlags32OptGroup;