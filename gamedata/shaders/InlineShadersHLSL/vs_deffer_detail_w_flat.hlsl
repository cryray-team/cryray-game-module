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

v2p_flat 	main (v_detail v)
{
	v2p_flat 		O;
	// index
	int 	i 	= v.misc.w;
	float4  m0 	= array[i+0];
	float4  m1 	= array[i+1];
	float4  m2 	= array[i+2];
	float4  c0 	= array[i+3];

	// Transform pos to world coords
	float4 	pos;
 	pos.x 		= dot	(m0, v.pos);
 	pos.y 		= dot	(m1, v.pos);
 	pos.z 		= dot	(m2, v.pos);
	pos.w 		= 1;

	// 
	float 	base 	= m1.w;
	float 	dp	= calc_cyclic   (dot(pos,wave));
	float 	H 	= pos.y - base;			// height of vertex (scaled)
	float 	frac 	= v.misc.z*consts.x;		// fractional
	float 	inten 	= H * dp;
	float2 	result	= calc_xz_wave	(dir2D.xz*inten,frac);
	pos		= float4(pos.x+result.x, pos.y, pos.z+result.y, 1);

#if SSFX_INT_GRASS > 0
	for (int b = 0; b < SSFX_INT_GRASS + 1; b++)
	{
		// Direction, Radius & Bending Strength, Distance and Height Limit
		float3 dir = benders_pos[b + 16].xyz;
		float3 rstr = float3(benders_pos[b].w, benders_pos[b + 16].ww);
		bool non_dynamic = rstr.x <= 0 ? true : false;
		float dist = distance(pos.xz, benders_pos[b].xz);
		float height_limit = 1.f - saturate(abs(pos.y - benders_pos[b].y) / ( non_dynamic ? 2.f : rstr.x ));
		height_limit *= H;

		// Adjustments ( Fix Radius or Dynamic Radius )
		rstr.x = non_dynamic ? benders_setup.x : rstr.x;
 		rstr.yz *= non_dynamic ? benders_setup.yz : 1.f;

		// Strength through distance and bending direction.
		float bend = 1.f - saturate(dist / (rstr.x + 0.001f));
		float3 bend_dir = normalize(pos.xyz - benders_pos[b].xyz) * bend;
		float3 dir_limit = dir.y >= -1 ? saturate(dot(bend_dir.xyz, dir.xyz) * 5.f) : 1.f; // Limit if nedeed

		// Apply direction limit
		bend_dir.xz *= dir_limit.xz;

		// Apply vertex displacement
		pos.xz += bend_dir.xz * 2.f * rstr.yy * height_limit; 			// Horizontal
		pos.y -= bend * 0.6f * rstr.z * height_limit * dir_limit.y;		// Vertical
	}
#endif

	// Normal in world coords
	float3 	norm;	//	= float3(0,1,0);
		norm.x 	= pos.x - m0.w	;
		norm.y 	= pos.y - m1.w	+ 0.75f;	// avoid zero
		norm.z	= pos.z - m2.w	;

	// Final out
	float4	Pp 	= mul		(m_WVP,	pos				);
	O.hpos 		= Pp;
	float3 orig		= mul		(m_WV,  normalize(norm)	);

	O.N = lerp(orig, mul((float3x3)m_WV,  v.pos), 0.25f);
	
	
	float3	Pe	= mul		(m_WV,  pos				);
//	O.tcdh 		= float4	((v.misc * consts).xy	);
	O.tcdh 		= float4	((v.misc * consts).xyyy );

	float 	_dp	= calc_cyclic   (dot(pos,wave*GRASS_WAVE_FREQ));
	O.tcdh.z	= consts.w+consts.z*max(0.f, _dp)*frac*GRASS_WAVE_POWER;

	O.position	= float4	(Pe, 		c0.w		);

	return O;
}
FXVS;
