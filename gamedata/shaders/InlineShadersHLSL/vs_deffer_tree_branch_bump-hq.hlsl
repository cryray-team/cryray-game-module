//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#include "Headers\common.h"

#ifdef SSFX_INT_GRASS
uniform float4 benders_pos[SSFX_INT_GRASS + 1];
uniform float4 benders_dir[SSFX_INT_GRASS + 1];
#endif

v2p_bumped 	main 	(v_tree I)
{
	I.Nh	=	unpack_D3DCOLOR(I.Nh);
	I.T		=	unpack_D3DCOLOR(I.T);
	I.B		=	unpack_D3DCOLOR(I.B);

	// Transform to world coords
	float3 	pos		= mul			(m_xform, I.P);

	//
	float 	base 	= m_xform._24	;		// take base height from matrix
	float 	dp		= calc_cyclic  	(wave.w+dot(pos,(float3)wave));
	float 	H 		= pos.y - base	;		// height of vertex (scaled, rotated, etc.)
	float 	frac 	= I.tc.z*consts.x;		// fractional (or rigidity)
	float 	inten 	= H * dp;				// intensity
	float2 	result	= calc_xz_wave	(wind.xz*inten*2.f, frac);
#ifdef		USE_TREEWAVE
			result	= 0.f;
#endif
	float4 	w_pos 	= float4(pos.x+result.x, pos.y, pos.z+result.y, 1);
	float2 	tc 		= (I.tc * consts).xy;
	
	// INTERACTIVE GRASS ( Bushes ) - SSS Update 15.4
	// https://www.moddb.com/mods/stalker-anomaly/addons/screen-space-shaders/
	
#ifdef SSFX_INT_GRASS
	for (int b = 0; b < SSFX_INT_GRASS + 1; b++)
	{
		// Direction, Radius & Bending Strength, Distance and Height Limit
		float3 dir = benders_dir[b].xyz;
		float3 rstr = float3(benders_pos[b].w, benders_dir[b].ww); // .x = Radius | .yz = Str
		bool non_dynamic = rstr.x <= 0 ? true : false;
		float dist = distance(w_pos.xz, benders_pos[b].xz);
		float height_limit = 1.0f - saturate(abs(pos.y - benders_pos[b].y) / ( non_dynamic ? 2.0f : rstr.x ));
		height_limit *= (1.0f - tc.y); // Bushes uses UV Coor instead of H to limit displacement

		// Adjustments ( Fix Radius or Dynamic Radius )
		rstr.x = non_dynamic ? 1.0f : rstr.x;
 		rstr.yz *= 1.0f;

		// Strength through distance and bending direction.
		float bend = 1.0f - saturate(dist / (rstr.x + 0.001f));
		float3 bend_dir = normalize(w_pos.xyz - benders_pos[b].xyz) * bend;
		float3 dir_limit = dir.y >= -1 ? saturate(dot(bend_dir.xyz, dir.xyz) * 5.0f) : 1.0f; // Limit if nedeed

		// Apply direction limit
		bend_dir.xz *= dir_limit.xz;

		// Apply vertex displacement
		w_pos.xz += bend_dir.xz * 2.25f * rstr.yy * height_limit; 		// Horizontal
		w_pos.y -= bend * 0.67f * rstr.z * height_limit * dir_limit.y;	// Vertical
	}
#endif
	
	//-' OldSerpskiStalker, soft edges fix
	float 	hemi 	= clamp(I.Nh.w * c_scale.w + c_bias.w, 0.3f, 1.f) * import_to_shader_params_cryray_3.r; // Limit hemi - SSS Update 14.5

	// Eye-space pos/normal
	v2p_bumped 		O;
	float3	Pe		= mul		(m_V,  	w_pos		);
	O.tcdh 			= tc.xy;
	O.hpos 			= mul		(m_VP,	w_pos		);
	O.position		= float4	(Pe, 	hemi		);

	// Calculate the 3x3 transform from tangent space to eye-space
	// TangentToEyeSpace = object2eye * tangent2object
	//		     = object2eye * transpose(object2tangent) (since the inverse of a rotation is its transpose)
	//Normal mapping

	// FLORA FIXES & IMPROVEMENTS - SSS Update 14
	// https://www.moddb.com/mods/stalker-anomaly/addons/screen-space-shaders/
	// Use real tree Normal, Tangent and Binormal.
	float3 N 	= unpack_bx4(I.Nh.xyz);
	float3 T 	= unpack_bx4(I.T.xyz);
	float3 B 	= unpack_bx4(I.B.xyz);
	float3x3 xform	= mul	((float3x3)m_xform_v, float3x3(
					T.x,B.x,N.x,
					T.y,B.y,N.y,
					T.z,B.z,N.z
				));

	// The pixel shader operates on the bump-map in [0..1] range
	// Remap this range in the matrix, anyway we are pixel-shader limited :)
	// ...... [ 2  0  0  0]
	// ...... [ 0  2  0  0]
	// ...... [ 0  0  2  0]
	// ...... [-1 -1 -1  1]
	// issue: strange, but it's slower :(
	// issue: interpolators? dp4? VS limited? black magic?

	// Feed this transform to pixel shader
	O.M1 			= xform[0];
	O.M2 			= xform[1];
	O.M3 			= xform[2];

#ifdef 	USE_TDETAIL
	O.tcdbump		= O.tcdh * dt_params;		// dt tc
#endif

	return O;
}
FXVS;