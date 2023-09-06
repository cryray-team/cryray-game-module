//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#ifndef        HMODEL_H
#define HMODEL_H


uniform float4		env_color;        // color.w  = lerp factor
float4 hmodel_stuff;  //x - hemi vibrance // y - hemi contrast // z - wet surface factor

void hmodel
(
	out float3 hdiffuse, out float3 hspecular, 
	float m, float h, float s, float3 Pnt, float3 normal
)
{
	normal = normalize(normal);	
	
// hscale - something like diffuse reflection
	float3	nw		= mul( m_inv_V, float4(normal, 1.f) );
	float	hscale	= h;	//. *        (.5h + .5h*nw.y);

// reflection vector
	float3	v2PntL	= normalize( Pnt );
	float3	v2Pnt	= mul( m_inv_V, float4(v2PntL, 1.f) );
	float3	vreflect= reflect( v2Pnt, nw );
	float	hspec	= 0.5f + 0.5f * dot( vreflect, v2Pnt );


// material	// sample material
	float4	light	= s_material.SampleLevel( smp_material, float3( hscale, hspec, m ), 0 ).xxxy;


// diffuse color
	float3	e0d		= env_s0.SampleLevel( smp_rtlinear, nw, 0 ).xyz;
	float3	e1d		= env_s1.SampleLevel( smp_rtlinear, nw, 0 ).xyz;
	float3	env_d	= env_color.xyz * lerp( e0d, e1d, env_color.w );
			env_d	*=env_d;
	
	hdiffuse= env_d * light.xyz + L_ambient.rgb;


// specular color
	float3 vreflectabs    = abs(vreflect);
    float  vreflectmax    = max(vreflectabs.x, max(vreflectabs.y, vreflectabs.z));
           vreflect      /= vreflectmax;
       if (vreflect.y < 0.999f)    
            vreflect.y= vreflect.y*2.f-1.f;     // fake remapping 
	float3	e0s		= env_s0.SampleLevel( smp_rtlinear, vreflect, 0 ).xyz;
	float3	e1s		= env_s1.SampleLevel( smp_rtlinear, vreflect, 0 ).xyz;
	float3	env_s	= env_color.xyz * lerp( e0s, e1s, env_color.w);
			env_s	*=env_s;
	float luminance = 1-dot( light.rgb, float3(LUMINANCE_VECTOR) );

	hspecular	= env_s*light.w*(s+(rain_params.x*0.25f))*luminance*15.f*(rain_params.x*2.f);
}
#endif