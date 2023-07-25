#pragma once
#include "../xrCore/xrCore.h"

typedef struct
{
    u32 u32ShaderPNVRender; // ��� ���������� ���

    bool bWeaponIsZoomed; // ��� ������� ���������, ��������� ��������� �������� ��� ������������
    bool bWpnDofReload; // ��� ����� ���� ��� �����������
    bool bHudAnimatorDof; // ��� ����� ���� ��� ������������� �����
    bool bLoadLevelAmbientsSounds; // �������� �� ��������

    bool bMoreArtefactsSlots; // 16 ������ �� �����
    bool bDisableBootScreens; // ��� ������������ ������

    bool bEnableSleep; // �������� ��������� ���
    bool bEnableThirst; // �������� ��������� �����

    float fEStunZoomWpn; // ���������� ��������� ��������� �� ��

    Fvector3 vNightvisionGammaAmbient;
    Fvector4 vNightvisionGammaHemi;

} SGameSettings;

extern ENGINE_API void vSGameSettingsInit();
extern ENGINE_API SGameSettings* pSGameSettings;