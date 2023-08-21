//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#ifndef	GBUFFER_STAGE_H
#define	GBUFFER_STAGE_H

//GBuffer targets struct
struct f_deffer        		
{
	float4 position : SV_Target0; //XY - Normal, Z - Depth, W - Hemi/M_ID
	float4 C : SV_Target1; //XYZ - Albedo, W - Specular
#ifdef EXTEND_F_DEFFER_INLINE
	uint mask : SV_COVERAGE;
#endif
};

//Unpacked data struct
struct gbuffer_data
{
	float3 P; //View space position
	float mtl; //Material ID
	
	float3 N; //Unpacked normal
	float hemi; //Hemi
	
	float3 C; //Albedo
	float gloss; //Specular
};


#ifdef INTERNAL_AMD
float2 gbuf_pack_normal( float3 norm )
{
   float2 res;

   res.x  = norm.z;
   res.y  = 0.5f * ( norm.x + 1.f ) ;
   res.y *= ( norm.y < 0.f ? -1.f : 1.f );

   return res;
}

float3 gbuf_unpack_normal( float2 norm )
{
   float3 res;

   res.z  = norm.x;
   res.x  = ( 2.f * abs( norm.y ) ) - 1.f;
   res.y = ( norm.y < 0 ? -1.f : 1.f ) * sqrt( abs( 1.f - res.x * res.x - res.z * res.z ) );

   return res;
}
#else
float2 gbuf_pack_normal( float3 norm )
{
    float2 res;
    res.x = atan2(norm.y, norm.x) / 3.14159f;
    res.y = norm.z;
    return res;
}

float3 gbuf_unpack_normal( float2 norm )
{
    float2 theta;
    sincos(norm.x * 3.14159f, theta.x, theta.y);
    float2 phi = float2(sqrt(1.f - norm.y * norm.y), norm.y);
    return float3(theta.y * phi.x, theta.x * phi.x, phi.y);
}
#endif

float gbuf_pack_hemi_mtl( float hemi, float mtl )
{
	uint packed_mtl = uint( ( mtl / 1.333333333f ) * 31.f );
	//	Clamp hemi max value
	uint packed = ( MUST_BE_SET + ( uint( saturate(hemi) * 255.9f ) << 13 ) + ( ( packed_mtl & uint( 31 ) ) << 21 ) );

	if( ( packed & USABLE_BIT_13 ) == 0 )
		packed |= USABLE_BIT_14;

	if( packed_mtl & uint( 16 ) )
		packed |= USABLE_BIT_15;

	return asfloat( packed );
}

float gbuf_unpack_hemi( float mtl_hemi )
{
	return float( ( asuint( mtl_hemi ) >> 13 ) & uint(255) ) * (1.f/254.8f);
}

float gbuf_unpack_mtl( float mtl_hemi )
{
   uint packed = asuint( mtl_hemi );
   uint packed_hemi = ( ( packed >> 21 ) & uint(15) ) + ( ( packed & USABLE_BIT_15 ) == 0 ? 0 : 16 );
   return float( packed_hemi ) * (1.f/31.f) * 1.333333333f;
}

//Packing stage
#ifndef EXTEND_F_DEFFER_INLINE
f_deffer pack_gbuffer( float4 norm, float4 pos, float4 col )
#else
f_deffer pack_gbuffer( float4 norm, float4 pos, float4 col, uint imask )
#endif
{
	f_deffer res;

	res.position = float4( gbuf_pack_normal( norm.xyz ), pos.z, gbuf_pack_hemi_mtl( norm.w, pos.w ) );
	res.C = col;

#ifdef EXTEND_F_DEFFER_INLINE
   res.mask = imask;
#endif

	return res;
}

//Unpacking stage
gbuffer_data gbuffer_load_data( float2 tc : TEXCOORD, float2 pos2d, uint iSample )
{
	gbuffer_data gbd;

#ifndef MSAA_ANTIALIASING_ENABLE
	float4 P = s_position.Sample( smp_nofilter, tc );
	float4 C = s_diffuse.Sample( smp_nofilter, tc );	
#else
	float4 P = s_position.Load( int3( pos2d, 0 ), iSample );
	float4 C = s_diffuse.Load( int3( pos2d, 0 ), iSample );
#endif

	// 3d view space pos reconstruction math
	gbd.P = float3( P.z * ( pos2d * pos_decompression_params.zw - pos_decompression_params.xy ), P.z );

	// reconstruct N
	gbd.N = gbuf_unpack_normal( P.xy );

	// reconstruct material
	gbd.mtl	= gbuf_unpack_mtl( P.w );

	// reconstruct hemi
	gbd.hemi = gbuf_unpack_hemi( P.w );

	gbd.C = C.xyz;
	gbd.gloss = C.w;

	return gbd;
}

gbuffer_data gbuffer_load_data( float2 tc : TEXCOORD, float2 pos2d )
{
   return gbuffer_load_data( tc, pos2d, 0 );
}

gbuffer_data gbuffer_load_data_offset( float2 tc : TEXCOORD, float2 OffsetTC : TEXCOORD, float2 pos2d )
{
	float2 delta = ( ( OffsetTC - tc ) * screen_res.xy );

	return gbuffer_load_data( OffsetTC, pos2d + delta, 0 );
}

gbuffer_data gbuffer_load_data_offset( float2 tc : TEXCOORD, float2 OffsetTC : TEXCOORD, float2 pos2d, uint iSample )
{
   float2 delta = ( ( OffsetTC - tc ) * screen_res.xy );

   return gbuffer_load_data( OffsetTC, pos2d + delta, iSample );
}

#endif	//GBUFFER_STAGE_H
