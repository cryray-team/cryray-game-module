//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

//screen_dimensions = resolution
//gbufferProjection = projeciton matrix
//cPosV = view position
//normalV = view normal
//cTexCoord = texture coordinates
//based on: https://www.activision.com/cdn/research/Practical_Real_Time_Strategies_for_Accurate_Indirect_Occlusion_NEW%20VERSION_COLOR.pdf

float3 calc_gtao(float3 cPosV, float3 normalV, float2 cTexCoord)
{
	float fov = atan(1.0 / m_P._m11);
	float proj_scale = float(pos_decompression_params2.y) / (tan(fov * 0.5f) * 2.f);
	float screen_radius = (GTAO_RADIUS * 0.5f * proj_scale) / cPosV.z;
	float3 viewV = -normalize(cPosV);

	float noiseOffset = GTAO_Offsets(cTexCoord);
	float noiseDirection = GTAO_Noise(cTexCoord * pos_decompression_params2.xy);

    float falloff_mul = 2.f / pow(GTAO_RADIUS, 2.f);
    float screen_res_mul = 1.f / GTAO_SAMPLE * pos_decompression_params2.zw;
    float pi_by_slices = 3.14f / GTAO_SLICES;

	float visibility = 0.f;

    for(int slice = 0; slice < GTAO_SLICES; slice++)
    {
        float phi = (slice + noiseDirection) * pi_by_slices;
        float2 omega = float2(cos(phi), sin(phi));
        float3 directionV = float3(omega.x, omega.y, 0.f);

        float3 orthoDirectionV = directionV - (dot(directionV, viewV) * viewV);
        float3 axisV = cross(directionV, viewV);
        float3 projNormalV = normalV - axisV * dot(normalV, axisV);
        float projNormalLenght = length(projNormalV);

        float sgnN = sign(dot(orthoDirectionV, projNormalV));
        float cosN = saturate(dot(projNormalV, viewV) / projNormalLenght);
        float n = sgnN * acos(cosN);
        float sinN2 = 2.f * sin(n);

        for(int side = 0; side < 2; side ++)
        {
            float cHorizonCos = -1.f;

            for(int sample = 0; sample < GTAO_SAMPLE; sample++)
            {
                float2 s = max( screen_radius * (float(sample) + noiseOffset), 4.f + float(sample)) * screen_res_mul; // fix for multiplying s by sample=0 from Unity code
                float2 sTexCoord = cTexCoord + (-1.f + 2.f * side) * s * float2(omega.x, -omega.y);
                float3 sPosV = unpack_position(sTexCoord);
                float3 sHorizonV = sPosV - cPosV;
                float falloff = saturate(dot(sHorizonV, sHorizonV) * falloff_mul);
                float H = dot(normalize(sHorizonV), viewV);
                cHorizonCos = (H > cHorizonCos) ? lerp(H, cHorizonCos, falloff) : cHorizonCos;
            }

            float h = n + clamp((-1.f + 2.f * side) * acos(cHorizonCos) - n, -3.14f/2.f, 3.14f/2.f);
            visibility += projNormalLenght * (cosN + h * sinN2 - cos(2.f * h - n)) / 4.f;
        }
    }

    visibility /= GTAO_SLICES;
    return float3(visibility, cPosV.z, 0.f);
}