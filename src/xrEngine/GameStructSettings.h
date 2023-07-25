#pragma once
#include "../xrCore/xrCore.h"

typedef struct
{
    u32 u32ShaderPNVRender; // Для шейдерного ПНВ

    bool bWeaponIsZoomed; // Для эффекта оглушения, отключает убываение значения при прицеливании
    bool bWpnDofReload; // Для смены дофа при перезарядке
    bool bHudAnimatorDof; // Для смены дофа при использовании итема
    bool bLoadLevelAmbientsSounds; // Эмбиенты на локациях

    bool bMoreArtefactsSlots; // 16 слотов на поясе
    bool bDisableBootScreens; // Для загрузочного экрана

    bool bEnableSleep; // Включить движковый сон
    bool bEnableThirst; // Включить движковую жажду

    float fEStunZoomWpn; // Сохранение параметра оглушения по ГГ

    Fvector3 vNightvisionGammaAmbient;
    Fvector4 vNightvisionGammaHemi;

} SGameSettings;

extern ENGINE_API void vSGameSettingsInit();
extern ENGINE_API SGameSettings* pSGameSettings;