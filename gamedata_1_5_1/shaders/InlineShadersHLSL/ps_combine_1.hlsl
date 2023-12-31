//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#include "Headers\h_common.hlsl"
#include "Headers\h_lmodel.hlsl"
#include "Headers\h_hmodel.hlsl"
#include "Headers\h_fog.hlsl"

// Check MODs
#include "ScreenSpaceAddon\h_screenspace_reflections.hlsl"
#include "ScreenSpaceAddon\h_screenspace_fog.hlsl"

struct	_input
{
	float4	tc0	: TEXCOORD0;	// tc.xy, tc.w = tonemap scale
	float2	tcJ	: TEXCOORD1;	// jitter coords
	float4 pos2d : SV_Position;
};

struct	_out
{
	float4	low		: SV_Target0;
	float4	high	: SV_Target1;
};

_out main ( _input I )
{
	gbuffer_data gbd = gbuffer_load_data( GLD_P(I.tc0.xy, I.pos2d.xy, ISAMPLE) );
	
	// Sample the buffers:
	float4	P = float4( gbd.P, gbd.mtl );	// position.(mtl or sun)
	float4	N = float4( gbd.N, gbd.hemi );		// normal.hemi
	float4	D = float4( gbd.C, gbd.gloss );		// rgb.gloss
	
	float4	L = s_accumulator.Sample( smp_nofilter, I.tc0.xy);	// diffuse.specular

	if (abs(P.w - MAT_FLORA) <= 0.05f) 
	{
		// Reapply distance factor to fix overtly glossy plants in distance
		// Unfortunately some trees etc don't seem to use the same detail shader
		float	fAtten = 1.f - smoothstep(0.f, 50.f, P.z);
		D.a	*= (fAtten * fAtten);
	}
	
     // static sun
	float mtl = P.w;

	// hemisphere
	float3 hdiffuse, hspecular;

	float3 occ = s_ambient_occlusion.Sample(smp_nofilter, I.tc0.xy);

	occ = compute_colored_ao(occ.x, D.xyz);
	
	hmodel	(hdiffuse, hspecular, mtl, N.w, D.w, P.xyz, N.xyz);
	
	// AO implementation
	hdiffuse *= (1.f - (1.f - occ) * (1.f - dot(hdiffuse.rgb, LUMINANCE_VECTOR)));

    float4 light = float4(L.rgb + hdiffuse, L.w)        ;
    float4 C = D*light;                             // rgb.gloss * light(diffuse.specular)
	float3 spec = C.www * L.rgb + hspecular * C.rgba.xyz;              // replicated specular

	float3       color     = C.rgb + spec;
	
    // here should be distance fog
	float3 WorldP = mul(m_inv_V, float4(P.xyz, 1));
	float4 linear_fog = get_linear_fog(P.xyz);
	float4 height_fog = get_height_fog(P.xyz);
		
	// SSR Implementation
#ifdef LAUNCHER_OPT_SSR_SURFACES
	SSFX_ScreenSpaceReflections(I.tc0, P, N, D.a, color, ISAMPLE);	
#endif

	color = lerp(color, height_fog.xyz, height_fog.w);	
	color = lerp(color, linear_fog.xyz, linear_fog.w);
	
	float		distance		= length		(P.xyz);		
	float		fog				= saturate		(distance*fog_params.w + fog_params.x);
				fog 			= SSFX_HEIGHT_FOG(P.xyz, WorldP.y, color);
				color			= lerp			(color,fog_color.xyz,fog);
					
    float       skyblend		= saturate		(fog*fog);

    float       tm_scale        = I.tc0.w;                // interpolated from VS

    _out        o;
		
	if (AO_debug)
		color = occ;

	tonemap(o.low, o.high, color, tm_scale);	
	o.low.a = skyblend;
	o.high.a = skyblend;

	return o;
}
