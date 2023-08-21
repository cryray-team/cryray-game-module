//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#ifndef COMMON_H
#define COMMON_H
#include "Headers\h_common_s.hlsl"
#include "ScreenSpaceAddon\h_settings_global.hlsl"

#include "Main\h_defines.hlsl"
#include "Main\h_import_params.hlsl"
#include "Main\h_sampler.hlsl"
#include "Main\h_uniforms_float2.hlsl"
#include "Main\h_uniforms_float3.hlsl"
#include "Main\h_uniforms_float3x4.hlsl"
#include "Main\h_uniforms_float4.hlsl"
#include "Main\h_uniforms_float4x4.hlsl"
#include "Main\h_Texture2D.hlsl"
#include "Main\h_Texture3D.hlsl"
#include "Main\h_TextureCube.hlsl"
#include "Main\h_float_func.hlsl"
#include "Main\h_float3_func.hlsl"

#include "Headers\h_common_defines.hlsl"
#include "Headers\h_common_policies.hlsl"
#include "Main\h_struct_deffer_flat.hlsl"
#include "Main\h_struct_v_aa.hlsl"
#include "Main\h_struct_shadows.hlsl"
#include "Main\h_struct_ssss.hlsl"
#include "Main\h_struct_bumped.hlsl"
#include "Main\h_struct_static.hlsl"
#include "Main\h_struct_bloom.hlsl"
#include "Main\h_struct_v2p_tl.hlsl"
#include "Main\h_struct_v_postpr.hlsl"
#include "Main\h_struct_v_filter.hlsl"
#include "Main\h_struct_v_dumb.hlsl"
#include "Main\h_struct_v_model.hlsl"
#include "Main\h_struct_v_tree.hlsl"
#include "Main\h_struct_v_detail.hlsl"
#include "Main\h_struct_p_volume.hlsl"
#include "Headers\h_common_samplers.hlsl"
#include "Headers\h_common_cbuffers.hlsl"
#include "Headers\h_common_functions.hlsl"
#if (defined(MSAA_ALPHATEST_HIGH) || defined(MSAA_ALPHATEST_LOW))
#include "Msaa\h_msaa_a_test.hlsl"
#endif
#include "Msaa\h_msaa_hdao_index.hlsl"
#include "Headers\h_gbuffer_stage.hlsl"

#define xmaterial float(L_material.w)

#define FXPS technique _render{pass _code{PixelShader=compile ps_3_0 main();}}
#define FXVS technique _render{pass _code{VertexShader=compile vs_3_0 main();}}
#endif
