#pragma once
#include "../xrCore/xrCore.h"

typedef struct
{
    /*Textures*/
    float QualityTransparentTextures; //-' 2^8 = size bit-field
    unsigned psLodTextures;
    /*Textures*/

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
    /*SunSettings*/

    /*Water*/
    bool SSRWaterEnbaled;
    bool bSSRSurfaces;
    bool bRenderPuddles;
    /*Water*/

    /*Indirect light*/
    bool bIndirectLight;
    unsigned uIndirectLightQuality;
    /*Indirect light*/

    /*Grass*/
    float fGrassDensity;
    float fGrassRadius;
    float fGrassHeight;
    /*Grass*/

    /*SSAO*/
    unsigned int GetIndexSSAOOverride;
    unsigned int SSAOQualityOverride;
    /*SSAO*/

    /*Other options render*/
    bool LightVolumetricEnable;
    bool SunMaskEnable;
    bool WetSurfaceEnable;
    bool MotionBlurEnable;
    bool bUseInlineReshade;
    bool bUseShaderPNV;
    bool bShadersDisasm;
    bool bSkipErrorsHLSL;
    /*Other options render*/

    /*Other options*/
    bool hlsl_compile_log;
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
    bool use_csharp_compiler;
    bool csharp_class_reg_debug;

    u32 uIntervalAutosaveManager;
    bool bStartTimerAutosave;
    /*Other options*/

    /*Limit FPS*/
    bool vsync;
    unsigned fps;
    /*Limit FPS*/

    /*CryRayVariables*/
    bool bCryRayEngineBorderlessWinStyle; // При безграничных типах окна блокируем сворачивания приложения
    BOOL BLockfpsIntro; // Фикс для выбора герцовки монитора

    DWORD hours; // Время от старта приложения, в часах
    DWORD minutes; // Время от старта приложения, в минутах
    DWORD seconds; // Время от старта приложения, в секундах
    /*CryRayVariables*/
} StructCryRay;

extern ENGINE_API void vStructCryRayInit();
extern ENGINE_API StructCryRay* pStructCryRay;