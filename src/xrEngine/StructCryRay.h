#pragma once
#include "../xrCore/xrCore.h"

typedef struct
{
    /*Textures*/
    float QualityTransparentTextures; //-' 2^8 = size bit-field
    unsigned psLodTextures;
    bool steep_parallax;
    bool detail_bump;
    /*Textures*/

    /*Flora*/
    bool bCollGrass;
    bool bCollBushesTree;
    /*Flora*/

    /*Tessellation*/
    bool WireFrame;
    unsigned int Enable : 1;
    unsigned int Quality : 8;
    /*Tessellation*/

    /*MSAA*/
    unsigned int MSAA_Token_override;
    unsigned int MSAAAtest_Token_override;

    unsigned int MSAA_Enable : 4;
    unsigned int MSAA_Atest : 8;
    unsigned int MSAA_Samples : 8;
    /*MSAA*/

    /*ScreenshotMode*/
    bool type_jpg;
    bool type_png;
    bool type_bmp;
    /*ScreenshotMode*/

    /*SunSettings*/
    unsigned int GetIndexQualityShadowsOverride;
    unsigned int GetIndexQualitySunShaftsOverride;
    bool bFPDepthOfShadows;
    bool bIL;
    bool bSSFXScreenSpaceShadows;
    /*SunSettings*/

    /*Water*/
    bool SSRWaterEnbaled;
    bool WaterSpeculars;
    bool WaterFoam;
    bool bSSRSurfaces;
    bool bRenderPuddles;
    /*Water*/

    /*SSAO*/
    bool bComputeColoredSSAO;
    unsigned int GetIndexSSAOOverride;
    unsigned int SSAOQualityOverride;
    /*SSAO*/

    /*Other options render*/
    bool LightVolumetricEnable;
    bool GrassWaveEnable;
    bool SunMaskEnable;
    bool WetSurfaceEnable;
    bool MotionBlurEnable;
    bool LanceBloomGRMEnable;
    bool bHDR;
    bool bUseInlineReshade;
    bool bUseShaderPNV;
    unsigned int GetIndexShadowMapOverride;
    bool bShadersDisasm;
    bool bSkipErrorsHLSL;
    /*Other options render*/

    /*Other options*/
    bool hlsl_compile_log;
    bool no_ram_textures;
    bool shaders_cache_enable;
    bool dbgdev;
    bool dbgbullet;
    bool dbgloadscripts;
    bool dbgactionplanerIC;
    bool dbgloadingcustomdata;
    bool dbgscriptstoragedebug;
    bool dbgr4dev;
    bool dbg;
    bool dbgnetspawn;
    bool noprefetch;
    bool inline_shaders;
    bool hard_ai_vis;
    bool hard_vis_npc;
    bool sounds_EFX;
    bool sounds_caching;
    bool csharp_class_reg_debug;
    bool bThreeBufferCount;
    bool bUseIK;

    u32 uIntervalAutosaveManager;
    bool bStartTimerAutosave;
    /*Other options*/

    /*Limit FPS*/
    unsigned fps;
    /*Limit FPS*/

    /*CryRayVariables*/
    bool bCryRayEngineBorderlessWinStyle; // При безграничных типах окна блокируем сворачивания приложения
    BOOL BLockfpsIntro; // Фикс для выбора герцовки монитора

    DWORD hours; // Время от старта приложения, в часах
    DWORD minutes; // Время от старта приложения, в минутах
    DWORD seconds; // Время от старта приложения, в секундах
    /*CryRayVariables*/

    /*CryRayCoreSettings*/
    bool LogCoreWorkersMultiplier;
    bool BlockCoreWorkersMultiplier;

    DWORD CoreWorkersMultiplier;
    /*CryRayCoreSettings*/
} StructCryRay;

extern ENGINE_API void vStructCryRayInit();
extern ENGINE_API StructCryRay* pStructCryRay;