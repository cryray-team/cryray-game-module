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

static const float ColorRatio = 1.1f; //-' Compliance with the color gamut
static const float ConstAmountClamp = 100.f; //-' ...

float4 main(v2p_aa_AA I) : SV_Target
{
    float2 center = I.Tex0;
    float4 img = s_image.Sample(smp_rtlinear, center);
	
    float amount = ((hit_effect.x / 50.f) * 0.33f);
    float amount2 = ((hit_effect.x / 60000.f) * 0.33f);

    float2 offset_fringe = distance(float2(0.5f, 0.5f), center) * float2(amount2, amount2);

    float lerp_amount = saturate(distance(center.xy, float2(0.5f, 0.5f)));
    float red_boost = lerp(amount, amount2, lerp_amount);

    red_boost = lerp(red_boost, 1.f, (amount / ConstAmountClamp));

    // Уменьшение яркости красного цвета
    img.r = (s_image.Sample(smp_rtlinear, center + offset_fringe).r / ColorRatio) + red_boost * 1.0f; // Например, умножение на 0.5 уменьшит яркость красного

    img.g = s_image.Sample(smp_rtlinear, center).g / ColorRatio;

    // Добавление виньетки (затемнение по бокам)
    float vignette = 1.0 - length(center - float2(0.5, 0.5)) * red_boost; // Создание затемнения от центра
    vignette = saturate(vignette); // Ограничение в диапазоне [0, 1]
    
    img.rgb *= vignette; // Применение затемнения

    img.rgb *= 1.4f - (amount * saturate(distance(center.xy, float2(0.5f, 0.5f))));

    return img;
}
