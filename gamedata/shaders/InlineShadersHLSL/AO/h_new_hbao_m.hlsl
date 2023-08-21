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

float horizon_occlusion2(float2 deltaUV, float2 uv0, float3 P, 
float numSteps, float randstep, float3 dPdu, float3 dPdv)
{
    // Randomize starting point within the first sample distance
    float2 uv = uv0 + snap_uv_offset( randstep * deltaUV );
    
    // Snap increments to pixels to avoid disparities between xy 
    // and z sample locations and sample along a line
    deltaUV = snap_uv_offset( deltaUV );

    // Compute tangent vector using the tangent plane
    float3 T = deltaUV.x * dPdu + deltaUV.y * dPdv;

    float tanH = tangent(T);
    float sinH = tanH / sqrt(1.f + tanH*tanH);

    float ao = 0.f;
    for(float j = 1.f; j <= numSteps; ++j) 
	{
        uv += deltaUV;
        float3 S = fetch_eye_pos(uv);
        
        // Ignore any samples outside the radius of influence
        float d2  = length2(S - P);
        float tanS = tangent(P, S);	
		
		[branch]
        if ((d2 < g_sqr_R) && (tanS > tanH)) 
		{
            // Accumulate AO between the horizon and the sample
            float sinS = tanS / sqrt(1.f + tanS*tanS);
            float r = sqrt(d2) * g_inv_R;
            ao += falloff(r) * (sinS - sinH);
                
            // Update the current horizon angle
            tanH = tanS;
            sinH = sinS;
        } 
    }

    return ao;
}

float4 calc_hbao(float z, float3 N, float2 tc0, float4 pos2d)
{
    float3 P = uv_to_eye(tc0, z);

	float2 step_size = float2(0.5f / 1024.0f, 0.5f / 768.0f)*kernel/max(z,1.3f);
  	float numSteps = min(g_NumSteps, min(step_size.x * g_Resolution.x, step_size.y * g_Resolution.y));
	float numDirs = min ( g_NumDir, min(step_size.x / 4.f * g_Resolution.x, step_size.y / 4.f * g_Resolution.y));
	
    if (numSteps < 1.f) 
		return 1.f;
	
    step_size = step_size / ( numSteps + 1.f );

// (cos(alpha),sin(alpha),jitter)
    float3 rand_Dir = jitter4.Load(int3((int)pos2d.x & 63, (int)pos2d.y & 63, 0)).xyz;

	// footprint optimization
	float maxNumSteps = g_MaxFootprintUV / step_size;
	if (maxNumSteps < numSteps)
	{
		numSteps = floor(maxNumSteps + rand_Dir.z);
        numSteps = max(numSteps, 1.f);
	    step_size = g_MaxFootprintUV / numSteps;
    }

    float4 tangentPlane = float4(N, dot(P, N));
    float3 Pr = tangent_eye_pos(tc0 + float2(g_InvResolution.x, 0.f), tangentPlane);
    float3 Pl = tangent_eye_pos(tc0 + float2(-g_InvResolution.x, 0.f), tangentPlane);
    float3 Pt = tangent_eye_pos(tc0 + float2(0.f, g_InvResolution.y), tangentPlane);
    float3 Pb = tangent_eye_pos(tc0 + float2(0.f, -g_InvResolution.y), tangentPlane);

    float3 dPdu = min_diff(P, Pr, Pl);
    float3 dPdv = min_diff(P, Pt, Pb) * (g_Resolution.y * g_InvResolution.x);

    // Loop for all directions
    float ao = 0.f;
    float alpha = 2.f * M_PI / g_NumDir;
    float delta = g_NumDir / numDirs;
    
    int   iNumDir = ceil(int(g_NumDir / delta));
    
    for (int i = 0; i < iNumDir; ++i ) 
	{  
        float d = float(i)*delta;
	    float angle = alpha * d;
	    float2 dir = float2(cos(angle), sin(angle));
	    float2 deltaUV =
		float2(dir.x*rand_Dir.x - dir.y*rand_Dir.y, dir.x*rand_Dir.y + dir.y*rand_Dir.x) * step_size.xy;
	    
	    ao += horizon_occlusion2(deltaUV, tc0, P, numSteps, rand_Dir.z, dPdu, dPdv);
	}

	float WeaponAttenuation = smoothstep(0.8f, 0.9f, length( P.xyz ));

    return 1.f - ao / g_NumDir * (g_Contrast*WeaponAttenuation);
}