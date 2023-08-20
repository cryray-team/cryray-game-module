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
#include "Headers\common_s.h"
#include "ScreenSpaceAddon\settings_global.h"

#include "Main\defines.h"
#include "Main\import_params.h"
#include "Main\sampler.h"
#include "Main\uniforms_float2.h"
#include "Main\uniforms_float3.h"
#include "Main\uniforms_float3x4.h"
#include "Main\uniforms_float4.h"
#include "Main\uniforms_float4x4.h"
#include "Main\Texture2D.h"
#include "Main\Texture3D.h"
#include "Main\TextureCube.h"
#include "Main\float_func.h"
#include "Main\float3_func.h"

#include "Headers\common_defines.h"
#include "Headers\common_policies.h"
#include "Main\struct_deffer_flat.h"
#include "Main\struct_v_aa.h"
#include "Main\struct_shadows.h"
#include "Main\struct_ssss.h"
#include "Main\struct_bumped.h"
#include "Main\struct_static.h"
#include "Main\struct_bloom.h"
#include "Main\struct_v2p_tl.h"
#include "Main\struct_v_postpr.h"
#include "Main\struct_v_filter.h"
#include "Main\struct_v_dumb.h"
#include "Main\struct_v_model.h"
#include "Main\struct_v_tree.h"
#include "Main\struct_v_detail.h"
#include "Main\struct_p_volume.h"
#include "Headers\common_samplers.h"
#include "Headers\common_cbuffers.h"
#include "Headers\common_functions.h"

#if (defined(MSAA_ALPHATEST_HIGH) || defined(MSAA_ALPHATEST_LOW))
#include "Msaa\msaa_a_test.h"
#endif
#include "Msaa\msaa_hdao_index.h"
#include "Headers\gbuffer_stage.h"

#define xmaterial float(L_material.w)

#define FXPS technique _render{pass _code{PixelShader=compile ps_3_0 main();}}
#define FXVS technique _render{pass _code{VertexShader=compile vs_3_0 main();}}
#endif
